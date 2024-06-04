#ifndef FRAMEQUEUE_H
#define FRAMEQUEUE_H

#include <QObject>

class FrameQueue : public QObject
{
    Q_OBJECT
public:
    explicit FrameQueue(QObject *parent = nullptr);

signals:
};

#endif // FRAMEQUEUE_H
