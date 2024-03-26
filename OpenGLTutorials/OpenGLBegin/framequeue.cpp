#include "framequeue.h"

FrameQueue::FrameQueue(QObject *parent)
    : QObject{parent}
{

}

FrameQueue::~FrameQueue()
{

}

void FrameQueue::pushFrame(YUVFrame frame)
{
    QMutexLocker locker(&mutex);
    m_queue.enqueue(frame);
}

YUVFrame FrameQueue::popFrame()
{
    QMutexLocker locker(&mutex);
    if(m_queue.size()>0){
        return m_queue.dequeue();
    }
    return {};
}

bool FrameQueue::hasFrame()
{
    QMutexLocker locker(&mutex);
    return m_queue.size()>0;
}

