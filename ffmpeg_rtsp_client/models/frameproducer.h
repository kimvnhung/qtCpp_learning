#ifndef FRAMEPRODUCER_H
#define FRAMEPRODUCER_H

#include <QObject>

class FrameProducer : public QObject
{
    Q_OBJECT
public:
    explicit FrameProducer(QObject *parent = nullptr);

signals:
};

#endif // FRAMEPRODUCER_H
