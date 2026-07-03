#include "videosynchronizer.h"

#include "log.h"
#include "videoconsumer.h"

#include <QThreadPool>

VideoSynchronizer::VideoSynchronizer(PlaybackClock* clock,
                                     std::shared_ptr<VideoFrameQueue> videoQueue)
    : videoQueue(videoQueue)
    , playbackClock(clock)
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

void VideoSynchronizer::processNext()
{
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

    auto frame = videoQueue->peek();
    bool isConsumed = false;

    if (frame)
    {
        int64_t pts = frame->get()->getPts();

        if (pts >= 0)
        {
            int64_t currentPts = playbackClock->currentTime();
            int64_t delay = pts - currentPts;

            if (abs(delay) <= 5)
            {
                consumeFrame(frame->get());
                isConsumed = true;
            }
            else if (delay < 0)
            {
                // Skip frames if they are too late
                LOGW() << "VideoSynchronizer: Skipping frame with PTS: " << pts
                       << " (Current PTS: " << currentPts << ")";
                isConsumed = true;
            }
            else
            {
                LOGD() << "VideoSynchronizer: Frame with PTS: " << pts
                       << " is ahead of current PTS: " << currentPts
                       << ". Waiting for synchronization.";
            }
        }
    }
    else
    {
        LOGW() << "VideoSynchronizer: No frame available in the queue.";
        QThread::msleep(10);
        isConsumed = true; // Indicate that we processed (or attempted to process) a frame, even if none was available
    }

    if (isConsumed)
    {
        auto deletable = videoQueue->pop(); // Remove the frame from the queue after processing or skipping
        deletable.reset(); // Ensure the frame is deleted to free memory
    }
}