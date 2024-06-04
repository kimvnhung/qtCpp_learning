#ifndef FRAMECONSUMER_H
#define FRAMECONSUMER_H

#include <QObject>

class FrameConsumer : public QObject
{
    Q_OBJECT
public:
    explicit FrameConsumer(QObject *parent = nullptr);

signals:
};

#endif // FRAMECONSUMER_H
