#ifndef TIMESTEP_H
#define TIMESTEP_H

#include <QObject>
#include "ruleline.h"

class TimeStep : public QObject
{
    Q_OBJECT
public:
    explicit TimeStep(QObject *parent = nullptr);

signals:
private:

};

#endif // TIMESTEP_H
