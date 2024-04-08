#include "ruleline.h"

#include <QDebug>

std::string convertMillisecondsToString(std::chrono::milliseconds duration) {
    using namespace std::chrono;

    // Convert milliseconds to seconds, minutes, hours, and remaining milliseconds
    auto totalSeconds = duration_cast<seconds>(duration).count();
    auto totalMinutes = duration_cast<minutes>(duration).count();
    auto totalHours = duration_cast<hours>(duration).count();

    // Calculate remaining milliseconds after converting to minutes
    auto remainingMillis = duration.count() % 1000;

    if (totalHours > 0) {
        return std::to_string(totalHours) + "h";
    } else if (totalMinutes > 0) {
        return std::to_string(totalMinutes) + "m";
    } else if (totalSeconds > 0) {
        return std::to_string(totalSeconds) + "s";
    } else {
        return std::to_string(remainingMillis) + "ms";
    }
}

RuleLine::RuleLine(QObject *parent, RuleLineType type, std::chrono::milliseconds value)
    :
    QObject{parent},
    m_type(type),
    m_text(QString::fromStdString(convertMillisecondsToString(value))),
    m_value(value)
{

}

QString RuleLine::text() const
{
    return m_text;
}

void RuleLine::setText(const QString &newText)
{
    if (m_text == newText)
        return;
    m_text = newText;
    emit textChanged();
}

RuleLine::RuleLineType RuleLine::type() const
{
    return m_type;
}

void RuleLine::setType(RuleLineType type)
{
    if(m_type == type)
        return;

    m_type = type;
    emit typeChanged();
}

double RuleLine::position() const
{
    return m_position;
}

void RuleLine::setPosition(double pos)
{
    qDebug()<<__FUNCTION__<<pos;
    if(m_position == pos)
        return;

    m_position = pos;
    emit positionChanged();
}


