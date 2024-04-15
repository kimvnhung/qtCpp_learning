#include "linedata.h"

LineData::LineData(QObject *parent, RuleLine::RuleLineType lineType, qint64 value, bool visible):
    QObject{parent},
    m_lineType(lineType),
    m_value(value),
    m_visible(visible)
{

}

int LineData::lineType() const
{
    return (int)m_lineType;
}

void LineData::setLineType(int value)
{
    if(((int)m_lineType) == value)
        return;

    m_lineType = (RuleLine::RuleLineType)value;
    if(m_visible)
        emit lineTypeChanged();
}

qint64 LineData::value() const
{
    return m_value;
}

void LineData::setValue(qint64 value)
{
    if(m_value == value)
        return;

    m_value = value;

    if(m_visible)
        emit valueChanged();
}

void LineData::setVisible(bool visible)
{
    if(m_visible == visible)
        return;

    m_visible = visible;
    if(m_visible)
    {
        emit valueChanged();
        emit lineTypeChanged();
    }
}

double LineData::position() const
{
    return m_position;
}

void LineData::setPosition(double value)
{
    if(m_position == value)
        return;

    m_position = value;
    emit positionChanged();
}


