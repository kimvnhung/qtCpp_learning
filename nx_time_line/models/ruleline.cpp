#include "ruleline.h"

RuleLine::RuleLine(QObject *parent, RuleLineType type, const QString& text, std::chrono::milliseconds pos)
    :
    QObject{parent},
    m_type(type),
    m_text(text),
    m_position(pos)
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

std::chrono::milliseconds RuleLine::position() const
{
    return m_position;
}

void RuleLine::setPosition(std::chrono::milliseconds pos)
{
    if(m_position == pos)
        return;

    m_position = pos;
    emit positionChanged();
}


