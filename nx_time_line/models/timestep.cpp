#include "timestep.h"

TimeStep::TimeStep(RulerContext* context):
    QObject{context},
    m_context(context)
{

}

RulerContext* TimeStep::context()
{
    return m_context;
}

QQmlListProperty<TimeStep> TimeStep::subItems()
{
    return QQmlListProperty<TimeStep>(this, &m_subItems);
}

int TimeStep::lineType() const
{
    return (int)m_lineType;
}

qint64 TimeStep::value() const
{
    return m_offset+m_unit;
}


