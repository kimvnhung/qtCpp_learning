#ifndef COMMON_H
#define COMMON_H

#include <QObject>

typedef qint32 Handle;

struct Frame
{
    const char* data;
    Handle m_handle;
};

#endif // COMMON_H
