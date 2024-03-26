#ifndef CONCURRENTQUEUE_H
#define CONCURRENTQUEUE_H

#include <QMutex>
#include <QObject>

template <typename T>
class ConcurrentQueue : public QObject
{
public:
    explicit ConcurrentQueue(QObject *parent = nullptr);
    ~ConcurrentQueue();

    void pushFrame(T frame);
    T popFrame();
    bool hasFrame();

private:
    QQueue<T> m_queue;
    QMutex mutex;
};

#endif // CONCURRENTQUEUE_H
