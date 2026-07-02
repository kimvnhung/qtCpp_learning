#include "videosynchronizer.h"

#include "log.h"

VideoSynchronizer::VideoSynchronizer(PlaybackClock* clock,
                                     std::shared_ptr<VideoFrameQueue> videoQueue)
    : videoQueue(videoQueue)
{
    Q_UNUSED(clock);
}

void VideoSynchronizer::start()
{
    LOGD();
}

void VideoSynchronizer::registerConsumer(VideoConsumer* consumer)
{
    LOGD() << "Registering VideoConsumer: " << consumer->name();
    consumers.push_back(consumer);
}