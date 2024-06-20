#ifndef LOG_H
#define LOG_H

#include <QDebug>

#define DEBUG(...) qDebug()<<__FUNCTION__<<__LINE__<<__VA_ARGS__;

#endif // LOG_H
