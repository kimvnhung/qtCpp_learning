#include "framequeue.h"

FrameQueue::FrameQueue(QObject* parent, int maxSize):
    QObject(parent),
    m_maxSize(maxSize)
{

}

FrameQueue::~FrameQueue()
{

}

void FrameQueue::waitForNotEmpty()
{
    const QMutexLocker locker(&mutex);
    IS_NOT_EMPTY.wait(&mutex);
}

void FrameQueue::waitForNotFull()
{
    const QMutexLocker locker(&mutex);
    IS_NOT_FULL.wait(&mutex);
}

void FrameQueue::notifyNotFull()
{
    IS_NOT_FULL.notify_all();
}

void FrameQueue::notifyNotEmpty()
{
    IS_NOT_EMPTY.notify_all();
}

bool FrameQueue::isFull()
{
    const QMutexLocker locker(&mutex);
    return frameQueue.size()==m_maxSize;
}

bool FrameQueue::isEmpty()
{
    const QMutexLocker locker(&mutex);
    return frameQueue.size()==0;
}

Frame FrameQueue::remove()
{
    const QMutexLocker locker(&mutex);
    Frame frame = frameQueue.dequeue();
    notifyNotFull();
    return frame;
}

void FrameQueue::addFrame(Frame frame)
{
    const QMutexLocker locker(&mutex);
    frameQueue.enqueue(frame);
    notifyNotEmpty();
}
