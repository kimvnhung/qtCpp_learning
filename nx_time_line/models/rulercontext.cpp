#include "rulercontext.h"

#include <QDebug>

double CHIGHEST_VISABLE_W = 250;
double CMAXIMUM_SCALE_W = 800;
qint64 CMS_LEVELS[] = {
    5000,  // 5s
    10000, // 10s
    30000,
    60000,
    300000,
    600000,
    1800000,
    3600000,
    10800000};

int CMS_LEVELS_SIZE = sizeof(CMS_LEVELS) / sizeof(qint64);

int CDELEGATE_STATES[][3] = {
    {2, 5, 10}, // 1s
    {2, 3, 2},  // 1any
    {5, 2, 5},  // 5s
    {5, 2, 3},  // 5min
    {2, 5, 2},  // 10any
    {3, 2, 5},  // 30any
    {3, 2, 3}   // 3h
};

RulerContext::RulerContext(QObject *parent, qint64 totalTime, qint64 highestUnit, double width, double x):
    QObject{parent},
    m_totalTime(totalTime),
    m_highestUnit(highestUnit),
    m_width(width),
    m_x(x)
{

}

RulerContext::~RulerContext()
{

}

double RulerContext::widthPerMili() const
{
    return m_widthPerMili;
}

void RulerContext::setWidthPerMili(double value)
{
    if(m_widthPerMili == value)
        return;

    m_widthPerMili = value;
    updateUnits();
    refreshVisibleRange();
    emit widthPerMiliChanged();
}

qint64 RulerContext::totalTime() const
{
    return m_totalTime;
}

void RulerContext::setTotalTime(qint64 newTotalTime)
{
    if(m_totalTime == newTotalTime)
        return;

    m_totalTime = newTotalTime;
    emit totalTimeChanged();
}

double RulerContext::width() const
{
    return m_width;
}

void RulerContext::setWidth(double newWidth)
{
    if(m_width == newWidth)
        return;

    m_width = newWidth;
    emit widthChanged();
    setWidthPerMili(newWidth/m_totalTime);
}

double RulerContext::x() const
{
    return m_x;
}

void RulerContext::setX(double newX)
{
    if(m_x == newX)
        return;

    m_x = newX;
    refreshVisibleRange();
    emit xChanged();
}

qint64 RulerContext::normalUnit() const
{
    return m_normalUnit;
}

void RulerContext::setNormalUnit(qint64 newNormalUnit)
{
    m_normalUnit = newNormalUnit;
}

qint64 RulerContext::smallUnit() const
{
    return m_smallUnit;
}

void RulerContext::setSmallUnit(qint64 newSmallUnit)
{
    m_smallUnit = newSmallUnit;
}

qint64 RulerContext::smallestUnit() const
{
    return m_smallestUnit;
}

void RulerContext::setSmallestUnit(qint64 newSmallestUnit)
{
    m_smallestUnit = newSmallestUnit;
}

qint64 RulerContext::highestUnit() const
{
    return m_highestUnit;
}

void RulerContext::setHighestUnit(qint64 newHighestUnit)
{
    m_highestUnit = newHighestUnit;
    refreshVisibleRange();
}

double RulerContext::visibleWidth() const
{
    return m_visibleWidth;
}

bool RulerContext::isRoundedBy(qint64 target, qint64 unit)
{
    return target % unit == 0;
}

qint64 RulerContext::roundedBy(qint64 target, qint64 unit)
{
    return target - (target % unit);
}

void RulerContext::setVisibleWidth(double newVisibleWidth)
{
    m_visibleWidth = newVisibleWidth;
    refreshVisibleRange();
}

void RulerContext::refreshVisibleRange()
{
    qint64 startedValue = roundedBy(abs(m_x) / widthPerMili(), highestUnit());
    qint64 stopedValue = roundedBy((abs(m_x) + m_visibleWidth) / widthPerMili(), highestUnit()) + highestUnit();
    m_visibleRange[0] = startedValue;
    m_visibleRange[1] = stopedValue;
    m_absoluteVisibleRange[0] = abs(m_x)/widthPerMili();
    m_absoluteVisibleRange[1] = (abs(m_x)+m_visibleWidth)/widthPerMili() + 1;
    qDebug()<<"visible range: "<<m_visibleRange[0]<<m_visibleRange[1]<<"x: "<<m_x<<"width: "<<m_width;
}

bool RulerContext::isVisible(qint64 value)
{
    return m_visibleRange[0] <= value && value <= m_visibleRange[1];
}

double RulerContext::relativeWidth(qint64 value)
{
    if(value < m_visibleRange[0] || value > m_visibleRange[1])
        return 0;

    if(value-m_visibleRange[0] < highestUnit())
        return (value-m_absoluteVisibleRange[0])*widthPerMili();

    if(m_visibleRange[1] - value < highestUnit())
        return (m_absoluteVisibleRange[1] - value)*widthPerMili();

    return highestUnit()*widthPerMili();
}

qint64 RulerContext::startValueByUnit(qint64 unit)
{
    return roundedBy(abs(m_x) / widthPerMili(), unit);
}

qint64 RulerContext::stopValueByUnit(qint64 unit)
{
    return roundedBy((abs(m_x)+m_visibleWidth) / widthPerMili(), unit)+unit;
}

qint64 RulerContext::absoluteStart() const
{
    return m_absoluteVisibleRange[0];
}

qint64 RulerContext::absoluteStop() const
{
    return m_absoluteVisibleRange[1];
}

double RulerContext::startPosition() const
{
    return m_visibleRange[0]*widthPerMili();
}

double RulerContext::stopPosition() const
{
    return m_visibleRange[1]*widthPerMili();
}

bool RulerContext::isMaximumScale() const
{
    return m_highestUnit==CMS_LEVELS[0] && m_highestUnit*m_widthPerMili > CMAXIMUM_SCALE_W;
}

void RulerContext::updateUnits()
{
    qDebug()<<__FUNCTION__;
    if(m_widthPerMili == 0)
        return;

    for (int i = 0; i < CMS_LEVELS_SIZE - 1; i++)
    {
        qDebug() << "widthPerMili*CMS_LEVE" << m_widthPerMili * CMS_LEVELS[i];
        if (m_widthPerMili * CMS_LEVELS[i] < CHIGHEST_VISABLE_W && CHIGHEST_VISABLE_W < m_widthPerMili * CMS_LEVELS[i + 1])
        {
            m_highestUnit = CMS_LEVELS[i + 1];
            break;
        }

        if (i == CMS_LEVELS_SIZE - 2)
            m_highestUnit = CMS_LEVELS[0];
    }

    int *delegateState;

    int secs = m_highestUnit / 1000;
    int mins = m_highestUnit / 60000;
    int hours = m_highestUnit / (3600000);

    if (secs == 1)
    {
        delegateState = CDELEGATE_STATES[0];
    }
    else if (secs == 5)
    {
        delegateState = CDELEGATE_STATES[2];
    }
    else if (mins == 5)
    {
        delegateState = CDELEGATE_STATES[3];
    }
    else if (hours == 3)
    {
        delegateState = CDELEGATE_STATES[6];
    }
    else if (secs == 10 || mins == 10 || hours == 10)
    {
        delegateState = CDELEGATE_STATES[4];
    }
    else if (secs == 30 || mins == 30 || hours == 30)
    {
        delegateState = CDELEGATE_STATES[5];
    }
    else if (mins == 1 || hours == 1)
    {
        delegateState = CDELEGATE_STATES[1];
    }

    int delegate0 = *(delegateState);
    int delegate1 = *(delegateState + 1);
    int delegate2 = *(delegateState + 2);

    m_smallestUnit = m_highestUnit / (delegate0 * delegate1 * delegate2);
    m_smallUnit = m_highestUnit / (delegate0 * delegate1);
    m_normalUnit = m_highestUnit / (delegate0);


    emit unitsChanged();
}
