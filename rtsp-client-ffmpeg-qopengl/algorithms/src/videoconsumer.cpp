#include "videoconsumer.h"

#include "log.h"

void VideoConsumer::renderFrame()
{
    auto frame = this->frame();

    if (frame)
    {
        auto videoFrame = std::dynamic_pointer_cast<VideoFrame>(frame);

        if (videoFrame)
        {
            LOGD() << "VideoConsumer(" << name() << "): Rendering VideoFrame - ID:" << videoFrame->id()
                   << "PTS:" << videoFrame->getPts()
                   << "Width:" << videoFrame->width()
                   << "Height:" << videoFrame->height();
        }
        else
        {
            LOGW() << "VideoConsumer: Received frame is not a VideoFrame.";
        }
    }
}

