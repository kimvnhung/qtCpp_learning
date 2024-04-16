#include "linedata.h"

LineData::LineData(QObject *parent, RuleLine::RuleLineType lineType, qint64 value, bool visible, int level):
    QObject{parent},
    m_lineType(lineType),
    m_value(value),
    m_visible(true),
    m_level(level)
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

    if(value >= (int)RuleLine::RuleLineType::UNDEFINED)
    {
        m_lineType = RuleLine::RuleLineType::UNDEFINED;
    }else if(value <= 0){
        m_lineType = RuleLine::RuleLineType::HIGHEST;
    }else{
        m_lineType = (RuleLine::RuleLineType)value;
    }

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

int LineData::level() const
{
    return m_level;
}

double LineData::relativePosition() const
{
    return m_relativePosition;
}

void LineData::setRelativePosition(double value)
{
    if(m_relativePosition == value)
        return;

    m_relativePosition = value;
    emit relativePositionChanged();
}

