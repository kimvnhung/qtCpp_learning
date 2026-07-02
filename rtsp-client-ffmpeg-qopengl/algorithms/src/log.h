#ifndef LOG_H
#define LOG_H

#include <QDebug>
#include <QString>
#include <QThread>
#include <QDateTime>

#define LOGD() qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz") << "[" << QThread::currentThreadId() << "]"
#define LOGW() qWarning() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz") << "[" << QThread::currentThreadId() << "]"
#endif // LOG_H
