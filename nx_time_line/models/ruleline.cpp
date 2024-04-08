#include "ruleline.h"

#include <QDebug>

std::string convertMillisecondsToString(std::chrono::milliseconds duration) {
    using namespace std::chrono;

    // Convert milliseconds to seconds, minutes, hours, and remaining milliseconds

    std::chrono::hours totalHours = duration_cast<hours>(duration);
    std::chrono::minutes totalMinutes = duration_cast<minutes>(duration-totalHours);
    std::chrono::seconds totalSeconds = duration_cast<seconds>(duration-totalHours-totalMinutes);
    std::chrono::milliseconds totalMiliseconds = duration_cast<milliseconds>(duration-totalHours-totalMinutes-totalSeconds);

    if (totalSeconds.count() > 0) {
        return std::to_string(totalSeconds.count()) + "s";
    } else if (totalMinutes.count() > 0) {
        return std::to_string(totalMinutes.count()) + "m";
    } else if (totalHours.count() > 0) {
        return std::to_string(totalHours.count()) + "h";
    } else {
        return std::to_string(totalMiliseconds.count()) + "ms";
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

std::chrono::milliseconds RuleLine::value() const
{
    return m_value;
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


