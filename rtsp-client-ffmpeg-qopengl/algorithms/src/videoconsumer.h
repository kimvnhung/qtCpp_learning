#ifndef VIDEOCONSUMER_H
#define VIDEOCONSUMER_H

#include <QString>
#include "iconsumer.h"

class VideoConsumer: public IConsumer
{
    // IConsumer interface
protected:
    void renderFrame() override;
};

#endif // VIDEOCONSUMER_H
