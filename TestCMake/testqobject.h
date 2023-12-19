#ifndef TESTQOBJECT_H
#define TESTQOBJECT_H

#include <QObject>

class TestQObject : public QObject
{
    Q_OBJECT
public:
    explicit TestQObject(QObject *parent = nullptr);

signals:
};

#endif // TESTQOBJECT_H
