#ifndef AUDIOCONSUMER_H
#define AUDIOCONSUMER_H

#include "iconsumer.h"


class AudioConsumer: public IConsumer
{
protected:
    void renderFrame() override ;
};

#endif // AUDIOCONSUMER_H
