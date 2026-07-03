#ifndef IFRAMECONSUMER_H
#define IFRAMECONSUMER_H

#include <QObject>
#include <memory>

#include "iframe.h"

class IFrameConsumer: public QObject
{
public:
    virtual void consume(IFrame* frame) = 0;
};

#endif // IFRAMECONSUMER_H
