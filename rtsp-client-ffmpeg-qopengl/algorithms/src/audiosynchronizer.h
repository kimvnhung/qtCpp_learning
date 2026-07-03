#ifndef AUDIOSYNCHRONIZER_H
#define AUDIOSYNCHRONIZER_H

#include "isynchronizer.h"
#include "audioconsumer.h"
#include "playbackclock.h"
#include "iframequeue.h"

#include <QMutex>

class AudioSynchronizer : public ISynchronizer
{
public:
    AudioSynchronizer(std::shared_ptr<AudioConsumer> consumer, PlaybackClock* clock,
                      std::shared_ptr<AudioFrameQueue> audioQueue, int64_t latency = 100);

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
    int64_t latency{100}; // Milliseconds

    QMutex mutex;
    bool is_processing{false};
    bool isProcessing();
    void setProcessing(bool newState);
};

#endif // AUDIOSYNCHRONIZER_H
