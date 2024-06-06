#ifndef ASYNCQUEUE_H
#define ASYNCQUEUE_H

#include <QMutex>
#include <QObject>
#include <QQueue>
#include <QWaitCondition>

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
            DBG("waiting for frame");
            condition.wait(&mutex);
        }

        T frame = m_queue.dequeue();
        condition.wakeOne();
        return frame;
    }

    void reset()
    {
        DBG("reset queue");
        m_isTerminate = false;
        //clear all in queue
        DBG("reset queue");
        m_queue.clear();
        DBG("reset queue");
        condition.wakeAll();
    }

    void terminate()
    {
        m_isTerminate = true;
        m_queue.clear();
        condition.wakeAll();
    }

private:
    QQueue<T> m_queue;
    QMutex mutex;
    QWaitCondition condition;
    int m_bufferSize = 0;
    bool m_isTerminate = false;
};

#endif // ASYNCQUEUE_H
