#ifndef VIDEOCONSUMER_H
#define VIDEOCONSUMER_H

#include <QQmlEngine>
#include "iframeconsumer.h"

class VideoConsumer: public IFrameConsumer
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(int id READ id NOTIFY frameChanged)
    Q_PROPERTY(qint64 pts READ pts NOTIFY frameChanged)
    Q_PROPERTY(int width READ width NOTIFY frameChanged)
    Q_PROPERTY(int height READ height NOTIFY frameChanged)
    // IFrameConsumer interface
public:
    void consume(IFrame *frame) override;

    int id() const;
    qint64 pts() const;
    int width() const;
    int height() const;
signals:
    void frameChanged();
private:

    VideoFrame *currentFrame = nullptr;
};

#endif // VIDEOCONSUMER_H
