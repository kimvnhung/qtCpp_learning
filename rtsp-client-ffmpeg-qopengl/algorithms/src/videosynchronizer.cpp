#include "videosynchronizer.h"

#include "log.h"
#include "videoconsumer.h"

#include <QThreadPool>

VideoSynchronizer::VideoSynchronizer(PlaybackClock* clock,
                                     std::shared_ptr<VideoFrameQueue> videoQueue, int64_t latency)
    : videoQueue(videoQueue)
    , playbackClock(clock)
    , latency(latency)
{

}

void VideoSynchronizer::consumeFrame(VideoFrame* frame)
{
    for (auto consumer : consumers)
    {
        auto videoConsumer = dynamic_cast<VideoConsumer *>(consumer);

        if (videoConsumer)
        {
            videoConsumer->consume(frame);
        }
    }
}

void VideoSynchronizer::registerConsumer(IFrameConsumer * consumer)
{
    LOGD() << "Registering VideoConsumer: ";
    consumers.push_back(consumer);
}

void VideoSynchronizer::onPause()
{
    LOGD() << "VideoSynchronizer: Playback paused.";
}

void VideoSynchronizer::onResume()
{
    LOGD() << "VideoSynchronizer: Playback resumed.";
}

void VideoSynchronizer::onSeek(double seconds)
{
    LOGD() << "VideoSynchronizer: Seeking to " << seconds << " seconds.";
}

bool VideoSynchronizer::isProcessing()
{
    QMutexLocker locker(&mutex);
    return is_processing;
}

void VideoSynchronizer::setProcessing(bool newState)
{
    QMutexLocker locker(&mutex);
    is_processing = newState;
}

void VideoSynchronizer::processNext()
{
    if (isProcessing())
    {
        LOGD() << "VideoSynchronizer: Already processing. Skipping this cycle.";
        return;
    }

    setProcessing(true);

    if (!playbackClock)
    {
        LOGW() << "VideoSynchronizer: Playback clock is null.";
        return;
    }

    if (!playbackClock->isPlaying())
    {
        LOGD() << "VideoSynchronizer: Playback is paused. Skipping frame processing.";
        return;
    }

    bool isConsumed = false;

    while (!isConsumed)
    {
        auto frame = videoQueue->peek();

        if (!frame)
        {
            LOGD() << "VideoSynchronizer: No more frames to process.";
            break;
        }

        int64_t pts = frame->get()->getPts();
        int64_t currentTime = playbackClock->currentTime();

        if (pts <= currentTime - latency)
        {
            // Skip this frame as it's too late to display it
            LOGD() << "VideoSynchronizer: Frame PTS " << pts << " is too late. Skipping.";
            videoQueue->pop();
            continue;
        }
        else if (pts > currentTime + latency)
        {
            // Frame is too early, wait for the right time
            LOGD() << "VideoSynchronizer: Frame PTS " << pts << " is too early. Waiting.";
            break;
        }
        else
        {
            // Frame is within the acceptable range, consume it
            LOGD() << "VideoSynchronizer: Consuming frame with PTS " << pts;
            consumeFrame(frame->get());
            videoQueue->pop();
            isConsumed = true;
        }
    }

    setProcessing(false);
}