#ifndef AUDIOSYNCHRONIZER_H
#define AUDIOSYNCHRONIZER_H

#include "isynchronizer.h"
#include "audioconsumer.h"
#include "playbackclock.h"
#include "iframequeue.h"

class AudioSynchronizer : public ISynchronizer
{
public:
    AudioSynchronizer(std::shared_ptr<AudioConsumer> consumer, PlaybackClock* clock,
                      std::shared_ptr<AudioFrameQueue> audioQueue);

    // ISynchronizer interface
public:
    void processNext() override;
    void onPause() override;
    void onResume() override;
    void onSeek(double seconds) override;

private:
    std::shared_ptr<AudioConsumer> audioConsumer;
    std::shared_ptr<AudioFrameQueue> audioQueue;
    std::shared_ptr<PlaybackClock> playbackClock;
};

#endif // AUDIOSYNCHRONIZER_H
