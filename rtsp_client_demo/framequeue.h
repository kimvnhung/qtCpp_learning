#ifndef FRAMEQUEUE_H
#define FRAMEQUEUE_H

#include <QMutex>
#include <QObject>
#include <QQueue>
#include <QWaitCondition>

#include "common.h"

class FrameQueue : public QObject
{
    Q_OBJECT
public:
    explicit FrameQueue(QObject *parent = nullptr,int maxSize = 10);
    ~FrameQueue();
    bool isFull();
    bool isEmpty();
    Frame remove();
    void waitForNotFull();
    void waitForNotEmpty();
    void addFrame(Frame frame);

private:
    QMutex mutex;
    QWaitCondition IS_NOT_FULL;
    QWaitCondition IS_NOT_EMPTY;
    int m_maxSize;
    QQueue<Frame> frameQueue;

    void notifyNotFull();
    void notifyNotEmpty();
};

#endif // FRAMEQUEUE_H
