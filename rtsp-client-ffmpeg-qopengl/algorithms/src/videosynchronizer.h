#ifndef VIDEOSYNCHRONIZER_H
#define VIDEOSYNCHRONIZER_H

#include "isynchronizer.h"

#include "playbackclock.h"
#include "iframequeue.h"

#include <QMutex>

class IFrameConsumer;
class VideoSynchronizer: public ISynchronizer
{
public:
    VideoSynchronizer(PlaybackClock* clock,
                      std::shared_ptr<VideoFrameQueue> videoQueue, int64_t latency = 100);

    void registerConsumer(IFrameConsumer* consumer);
private:
    std::vector<IFrameConsumer *> consumers;
    std::shared_ptr<VideoFrameQueue> videoQueue;
    PlaybackClock *playbackClock{nullptr};

    QMutex mutex;
    bool is_processing{false};
    bool isProcessing();
    void setProcessing(bool newState);

    int64_t latency{100}; // Milliseconds

    void consumeFrame(VideoFrame* frame);
    // QThread interface

    // ISynchronizer interface
public:
    void processNext() override;
    void onPause() override;
    void onResume() override;
    void onSeek(double seconds) override;
};

#endif // VIDEOSYNCHRONIZER_H
