#include "audiosynchronizer.h"

#include "log.h"

AudioSynchronizer::AudioSynchronizer(std::shared_ptr<AudioConsumer> consumer, PlaybackClock* clock,
                                     std::shared_ptr<AudioFrameQueue> audioQueue)
    : audioConsumer(consumer)
    , audioQueue(audioQueue)
{
    Q_UNUSED(clock);
}

void AudioSynchronizer::start()
{
    LOGD();
}
