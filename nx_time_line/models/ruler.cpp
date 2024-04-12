#include "ruler.h"

Ruler::Ruler(QObject *parent, qint64 totalTime):
    QObject{parent},
    m_totalTime(totalTime)
{
    updateLineDatas();
}

Ruler::~Ruler()
{

}

QQmlListProperty<LineData> Ruler::lineDatas()
{
    return QQmlListProperty<LineData>(this,&m_lineDatas);
}

double Ruler::x() const
{
    return m_x;
}

void Ruler::setX(double value)
{
    if(m_x == value)
        return;

    m_x = value;
    updateLineDatas();
    emit xChanged();
}

double Ruler::width() const
{
    return m_width;
}

void Ruler::setWidth(double value)
{
    if(m_width == value)
        return;

    m_width = value;
    updateLineDatas();
    emit widthChanged();
}

double Ruler::visibleWidth() const
{
    return m_visibleWidth;
}

void Ruler::setVisibleWidth(double value)
{
    if(m_visibleWidth == value)
        return;

    m_visibleWidth = value;
    updateLineDatas();
    emit visibleWidthChanged();
}

//private
void Ruler::updateLineDatas()
{
    if(m_width ==0 || m_visibleWidth == 0 || m_totalTime == 0)
        return;

    m_lineDatas.append(new LineData(this,RuleLine::RuleLineType::HIGHEST,1000,true));
}

