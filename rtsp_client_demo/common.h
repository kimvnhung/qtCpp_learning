#ifndef COMMON_H
#define COMMON_H

#include <QObject>


typedef qint32 Handle;

struct Frame
{
    const char* data;
    int dataSize;
    Handle m_handle;
};

#endif // COMMON_H
