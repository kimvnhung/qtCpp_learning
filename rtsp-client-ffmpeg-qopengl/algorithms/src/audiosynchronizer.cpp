#include "audiosynchronizer.h"

#include "log.h"

AudioSynchronizer::AudioSynchronizer(std::shared_ptr<AudioConsumer> consumer, PlaybackClock* clock,
                                     std::shared_ptr<AudioFrameQueue> audioQueue, int64_t latency)
    : audioConsumer(consumer)
    , audioQueue(audioQueue)
    , playbackClock(std::move(clock))
    , latency(latency)
{

}

bool AudioSynchronizer::isProcessing()
{
    QMutexLocker locker(&mutex);
    return is_processing;
}

void AudioSynchronizer::setProcessing(bool newState)
{
    QMutexLocker locker(&mutex);
    is_processing = newState;
}

void AudioSynchronizer::processNext()
{
    if (isProcessing())
    {
        LOGD() << "AudioSynchronizer: Already processing. Skipping this cycle.";
        return;
    }

    setProcessing(true);

    if (!playbackClock)
    {
        LOGW() << "AudioSynchronizer: Playback clock is null.";
        return;
    }

    if (!playbackClock->isPlaying())
    {
        LOGD() << "AudioSynchronizer: Playback is paused. Skipping frame processing.";
        return;
    }

    bool isConsumed = false;

    while (!isConsumed)
    {
        auto frame = audioQueue->peek(); // Peek at the next frame without removing it

        if (!frame)
        {
            LOGD() << "AudioSynchronizer: No more audio frames to process.";
            break;
        }

        int64_t pts = frame->get()->getPts();
        int64_t currentTime = playbackClock->currentTime();

        if (pts <= currentTime - latency)
        {
            // Skip this frame as it's too late to display it
            LOGD() << "AudioSynchronizer: Skipping audio frame with PTS " << pts << " as it's too late.";
            audioQueue->pop(); // Remove the frame from the queue
            continue;
        }
        else if (pts > currentTime + latency)
        {
            // The frame is too early to be played, wait for the next cycle
            LOGD() << "AudioSynchronizer: Audio frame with PTS " << pts << " is too early. Waiting.";
            break;
        }
        else
        {
            // The frame is within the acceptable latency range, consume it
            LOGD() << "AudioSynchronizer: Consuming audio frame with PTS " << pts << ".";
            audioConsumer->consume(frame->get());
            audioQueue->pop(); // Remove the frame from the queue after consuming
            isConsumed = true;
        }
    }

    setProcessing(false);
}

void AudioSynchronizer::onPause()
{
    LOGD() << "AudioSynchronizer: Playback paused.";
}

void AudioSynchronizer::onResume()
{
    LOGD() << "AudioSynchronizer: Playback resumed.";
}

void AudioSynchronizer::onSeek(double seconds)
{
    LOGD() << "AudioSynchronizer: Seeking to " << seconds << " seconds.";
}