#include "audioconsumer.h"

#include "log.h"

void AudioConsumer::renderFrame()
{
    auto frame = this->frame();

    if (frame)
    {
        auto audioFrame = std::dynamic_pointer_cast<AudioFrame>(frame);

        if (audioFrame)
        {
            LOGD() << "AudioConsumer(" << name() << "): Rendering AudioFrame - ID:" << audioFrame->id()
                   << "PTS:" << audioFrame->getPts()
                   << "SampleRate:" << audioFrame->sampleRate()
                   << "Channels:" << audioFrame->channels();
        }
        else
        {
            LOGW() << "AudioConsumer: Received frame is not an AudioFrame.";
        }
    }
}
