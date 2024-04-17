#ifndef BASECONTEXTOBJECT_H
#define BASECONTEXTOBJECT_H

#include <QObject>

class BaseContextObject : public QObject
{
    Q_OBJECT
public:
    explicit BaseContextObject(QObject *parent = nullptr);

signals:
};

#endif // BASECONTEXTOBJECT_H
