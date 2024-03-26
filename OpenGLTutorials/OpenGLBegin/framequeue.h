#ifndef FRAMEQUEUE_H
#define FRAMEQUEUE_H

#include "data_type.h"
#include <QMutex>
#include <QObject>
#include <QQueue>

class FrameQueue : public QObject
{
    Q_OBJECT
public:
    explicit FrameQueue(QObject *parent = nullptr);
    ~FrameQueue();

    void pushFrame(YUVFrame frame);

    YUVFrame popFrame();

    bool hasFrame();

private:
    QQueue<YUVFrame> m_queue;
    QMutex mutex;
};

#endif // FRAMEQUEUE_H
