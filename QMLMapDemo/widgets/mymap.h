#ifndef MYMAP_H
#define MYMAP_H

#include <QObject>

class MyMap : public QObject
{
    Q_OBJECT
public:
    explicit MyMap(QObject *parent = nullptr);

signals:
};

#endif // MYMAP_H
