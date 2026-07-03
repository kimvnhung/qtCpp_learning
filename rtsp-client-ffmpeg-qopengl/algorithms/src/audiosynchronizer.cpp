#include "audiosynchronizer.h"

#include "log.h"

AudioSynchronizer::AudioSynchronizer(std::shared_ptr<AudioConsumer> consumer, PlaybackClock* clock,
                                     std::shared_ptr<AudioFrameQueue> audioQueue)
    : audioConsumer(consumer)
    , audioQueue(audioQueue)
    , playbackClock(std::move(clock))
{

}

void AudioSynchronizer::processNext()
{
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

    auto frame = audioQueue->peek();
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
                audioConsumer->setFrame(frame.value());
                isConsumed = true;
            }
            else if (delay < 0)
            {
                // Skip frames if they are too late
                LOGW() << "AudioSynchronizer: Frame with PTS: " << pts
                       << " is behind current PTS: " << currentPts
                       << ". Skipping frame.";
                isConsumed = true;
            }
            else
            {
                LOGD() << "AudioSynchronizer: Frame with PTS: " << pts
                       << " is ahead of current PTS: " << currentPts
                       << ". Waiting for synchronization.";
            }
        }
    }
    else
    {
        LOGW() << "AudioSynchronizer: No audio frame available to process.";
    }

    if (isConsumed)
    {
        auto temp = audioQueue->pop();
        temp.reset();
    }
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