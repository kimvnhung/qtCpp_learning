#ifndef LOG_H
#define LOG_H

#include <QDebug>
#include <QThread>
#include <QString>

#define DBG(...) qDebug()<<QThread::currentThread()<<__FUNCTION__<<__LINE__<<__VA_ARGS__
#define WRN(...) qWarning()<<QThread::currentThread()<<__FUNCTION__<<__LINE__<<__VA_ARGS__

#endif // LOG_H
