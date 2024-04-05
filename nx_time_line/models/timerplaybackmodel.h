#ifndef TIMERPLAYBACKMODEL_H
#define TIMERPLAYBACKMODEL_H

#include "ruleline.h"
#include <QObject>
#include <chrono>

class TimerPlaybackModel : public QObject
{
    Q_OBJECT
public:
    explicit TimerPlaybackModel(QObject *parent = nullptr);

signals:

private:
    QList<RuleLine*> m_ruleLines;
    std::chrono::milliseconds m_duration;
};

#endif // TIMERPLAYBACKMODEL_H
