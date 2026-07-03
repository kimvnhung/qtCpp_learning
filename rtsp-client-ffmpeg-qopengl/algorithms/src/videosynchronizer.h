#ifndef VIDEOSYNCHRONIZER_H
#define VIDEOSYNCHRONIZER_H

#include "isynchronizer.h"

#include "videoconsumer.h"
#include "playbackclock.h"
#include "iframequeue.h"

class VideoSynchronizer: public ISynchronizer
{
public:
    VideoSynchronizer(PlaybackClock* clock,
                      std::shared_ptr<VideoFrameQueue> videoQueue);

    void registerConsumer(VideoConsumer* consumer);
private:
    std::vector<VideoConsumer *> consumers;
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
