#ifndef VIDEOSYNCHRONIZER_H
#define VIDEOSYNCHRONIZER_H

#include "videoconsumer.h"
#include "playbackclock.h"
#include "iframequeue.h"

class VideoSynchronizer
{
public:
    VideoSynchronizer(PlaybackClock* clock,
                      std::shared_ptr<VideoFrameQueue> videoQueue);
    void start();
    void registerConsumer(VideoConsumer* consumer);
private:
    std::vector<VideoConsumer *> consumers;
    std::shared_ptr<VideoFrameQueue> videoQueue;
};

#endif // VIDEOSYNCHRONIZER_H
