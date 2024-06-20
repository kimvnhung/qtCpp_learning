#include <QCoreApplication>
#include <QFuture>
#include <QThreadPool>
#include <QtConcurrent>
#include "async_queue.h"
#include "log.h"

AsyncQueue<int> queue;
QElapsedTimer timer;
qint64 startTime = timer.elapsed();

void produce()
{
    int count = 0;
    while (true) {
        DEBUG("Producing..."<<(timer.elapsed()-startTime));
        queue.enqueue(count++);
        QThread::msleep(500);
    }
}

void consume()
{
    while (true) {
        int value = queue.dequeue();
        if(value == NULL)
            continue;

        DEBUG(QThread::currentThreadId()<<"Consuming: " << value <<(timer.elapsed()-startTime));
    }
}



int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug() << "Main thread ID:" << QThread::currentThreadId();
    int TEST_COUNT = 16;
    queue.setRegisteredConsumer(TEST_COUNT);
    // Run a task asynchronously using QtConcurrent
    QFuture<void> producer = QtConcurrent::run(produce);
    for (int i = 0; i < TEST_COUNT; i++ ) {
        QFuture<void> consumer = QtConcurrent::run(consume);
    }

    return a.exec();
}
