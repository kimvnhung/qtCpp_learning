#include "audioconsumer.h"

#include "log.h"

void AudioConsumer::consume(IFrame *frame)
{
    if (!frame)
    {
        LOGW() << "AudioConsumer: Received null frame.";
        return;
    }

    currentFrame = dynamic_cast<AudioFrame *>(frame);

    if (!currentFrame)
    {
        LOGW() << "AudioConsumer: Frame is not an AudioFrame.";
        return;
    }

    emit frameChanged();
}

int AudioConsumer::id() const
{
    return currentFrame ? currentFrame->id() : -1;
}

qint64 AudioConsumer::pts() const
{
    return currentFrame ? currentFrame->getPts() : -1;
}

int AudioConsumer::sampleRate() const
{
    return currentFrame ? currentFrame->sampleRate() : 0;
}

int AudioConsumer::channels() const
{
    return currentFrame ? currentFrame->channels() : 0;
}