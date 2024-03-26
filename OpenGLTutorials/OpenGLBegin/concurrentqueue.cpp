#include "concurrentqueue.h"

template <typename T>
ConcurrentQueue<T>::ConcurrentQueue(QObject *parent)
    : QObject{parent}
{

}

template <typename T>
ConcurrentQueue<T>::~ConcurrentQueue()
{

}

template <typename T>
void ConcurrentQueue<T>::pushFrame(T frame)
{
    QMutexLocker locker(&mutex);
    m_queue.enqueue(frame);
}

template <typename T>
T ConcurrentQueue<T>::popFrame()
{
    QMutexLocker locker(&mutex);
    if(m_queue.size()>0){
        return m_queue.dequeue();
    }
    return {};
}

template <typename T>
bool ConcurrentQueue<T>::hasFrame()
{
    QMutexLocker locker(&mutex);
    return m_queue.size()>0;
}
