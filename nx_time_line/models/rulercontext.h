#ifndef RULERCONTEXT_H
#define RULERCONTEXT_H

#include <QMutex>
#include <QObject>

class RulerContext : public QObject
{
    Q_OBJECT
public:
    explicit RulerContext(QObject *parent = nullptr);
private:
};

#endif // RULERCONTEXT_H
