#ifndef AUDIOCONSUMER_H
#define AUDIOCONSUMER_H

#include <QQuickItem>
#include "iframeconsumer.h"


class AudioConsumer: public IFrameConsumer
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(int id READ id NOTIFY frameChanged)
    Q_PROPERTY(qint64 pts READ pts NOTIFY frameChanged)
    Q_PROPERTY(int sampleRate READ sampleRate NOTIFY frameChanged)
    Q_PROPERTY(int channels READ channels NOTIFY frameChanged)

public:
    void consume(IFrame *frame) override;

    int id() const;
    qint64 pts() const;
    int sampleRate() const;
    int channels() const;
signals:
    void frameChanged();
private:
    AudioFrame *currentFrame = nullptr;
};

#endif // AUDIOCONSUMER_H
