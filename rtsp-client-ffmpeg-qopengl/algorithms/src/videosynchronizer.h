#ifndef VIDEOSYNCHRONIZER_H
#define VIDEOSYNCHRONIZER_H

#include "isynchronizer.h"

#include "playbackclock.h"
#include "iframequeue.h"

class IFrameConsumer;
class VideoSynchronizer: public ISynchronizer
{
public:
    VideoSynchronizer(PlaybackClock* clock,
                      std::shared_ptr<VideoFrameQueue> videoQueue);

    void registerConsumer(IFrameConsumer* consumer);
private:
    std::vector<IFrameConsumer *> consumers;
    std::shared_ptr<VideoFrameQueue> videoQueue;
    PlaybackClock *playbackClock{nullptr};
    bool is_running{false};

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
