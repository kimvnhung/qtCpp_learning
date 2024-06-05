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
        while (queue.size() >= m_bufferSize){
            condition.wait(&mutex);
        }

        queue.enqueue(frame);
        condition.wakeAll();
    }

    int count()
    {
        QMutexLocker locker(&mutex);
        return queue.size();
    }

    T dequeue()
    {
        QMutexLocker locker(&mutex);
        while (queue.isEmpty()){
            condition.wait(&mutex);
        }

        return queue.dequeue();
    }

    void terminate()
    {
        QMutexLocker locker(&mutex);
        queue.clear();
        condition.wakeAll();
    }

private:
    QQueue<T> queue;
    QMutex mutex;
    QWaitCondition condition;
    int m_bufferSize = 0;
};

#endif // ASYNCQUEUE_H
