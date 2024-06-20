#ifndef ASYNCQUEUE_H
#define ASYNCQUEUE_H

#include <QMutex>
#include <QObject>
#include <QQueue>
#include <QWaitCondition>
#include "log.h"

template<typename T>
class AsyncQueue : public QObject
{
public:
    AsyncQueue(QObject *parent = nullptr, int bufferSize = 10)
        : QObject{parent}, m_bufferSize{bufferSize}
    {

    }

    ~AsyncQueue()
    {
        terminate();
    }

    void enqueue(const T &frame)
    {
        QMutexLocker locker(&mutex);
        while (m_queue.size() >= m_bufferSize){
            condition.wait(&mutex);
        }

        m_queue.enqueue(frame);
        condition.wakeOne();
    }

    int count()
    {
        QMutexLocker locker(&mutex);
        return m_queue.size();
    }

    T dequeue()
    {
        QMutexLocker locker(&mutex);
        while (m_queue.isEmpty()){
            if(m_isTerminate)
                return NULL;
            // DEBUG("waiting for frame");
            condition.wait(&mutex);
        }

        if(++m_consumed >= m_registeredConsumer)
        {
            m_consumed = 0;
            T frame = m_queue.dequeue();
            condition.wakeOne();
            return frame;
        }
        else
            return m_queue.front();
    }

    void pop()
    {
        QMutexLocker locker(&mutex);
        if(m_queue.isEmpty())
            return;

        m_queue.dequeue();
        condition.wakeOne();
    }

    T front()
    {
        QMutexLocker locker(&mutex);
        while (m_queue.isEmpty()){
            if(m_isTerminate)
                return NULL;
            // DEBUG("waiting for frame");
            condition.wait(&mutex);
        }

        return m_queue.front();
    }

    void reset()
    {
        // DEBUG("reset queue");
        m_isTerminate = false;
        //clear all in queue
        // DEBUG("reset queue");
        m_queue.clear();
        // DEBUG("reset queue");
        condition.wakeAll();
    }

    void terminate()
    {
        m_isTerminate = true;
        m_queue.clear();
        condition.wakeAll();
    }

    void setRegisteredConsumer(int consumer)
    {
        QMutexLocker locker(&mutex);
        if(consumer >= 1)
            m_registeredConsumer = consumer;
    }

private:
    QQueue<T> m_queue;
    QMutex mutex;
    QWaitCondition condition;
    int m_bufferSize = 0;
    bool m_isTerminate = false;

    int m_consumed = 0;
    int m_registeredConsumer = 1;
};

#endif // ASYNCQUEUE_H

