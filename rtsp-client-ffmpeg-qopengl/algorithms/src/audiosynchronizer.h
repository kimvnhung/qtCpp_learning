#ifndef AUDIOSYNCHRONIZER_H
#define AUDIOSYNCHRONIZER_H

#include "audioconsumer.h"
#include "playbackclock.h"
#include "iframequeue.h"

class AudioSynchronizer
{
public:
    AudioSynchronizer(std::shared_ptr<AudioConsumer> consumer, PlaybackClock* clock,
                      std::shared_ptr<AudioFrameQueue> audioQueue);

    void start();
private:
    std::shared_ptr<AudioConsumer> audioConsumer;
    std::shared_ptr<AudioFrameQueue> audioQueue;
};

#endif // AUDIOSYNCHRONIZER_H
