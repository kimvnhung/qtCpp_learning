#include "videoconsumer.h"

#include "log.h"

void VideoConsumer::consume(IFrame *frame)
{
    if (!frame)
    {
        LOGW() << "VideoConsumer: Received null frame.";
        return;
    }

    currentFrame = dynamic_cast<VideoFrame *>(frame);

    if (!currentFrame)
    {
        LOGW() << "VideoConsumer: Frame is not a VideoFrame.";
        return;
    }

    emit frameChanged();
}

int VideoConsumer::id() const
{
    return currentFrame ? currentFrame->id() : -1;
}

qint64 VideoConsumer::pts() const
{
    return currentFrame ? currentFrame->getPts() : -1;
}

int VideoConsumer::width() const
{
    return currentFrame ? currentFrame->width() : 0;
}

int VideoConsumer::height() const
{
    return currentFrame ? currentFrame->height() : 0;
}