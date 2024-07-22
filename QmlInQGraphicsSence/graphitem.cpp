#include "graphitem.h"

#include <QRectF>

int GraphItem::DISTANCE_UNIT = 100;

GraphItem::GraphItem(int x, int y, QObject *parent)
    : QObject{parent}
    , m_x{x}
    , m_y{y}
{

}

GraphItem::~GraphItem()
{

}

int GraphItem::x() const
{
    return m_x;
}

int GraphItem::y() const
{
    return m_y;
}

void GraphItem::setX(int x)
{
    if (m_x == x)
        return;

    m_x = x;
    emit xChanged();
}

void GraphItem::setY(int y)
{
    if (m_y == y)
        return;

    m_y = y;
    emit yChanged();
}

QRectF GraphItem::boundingRect() const
{
    return QRectF((x()-1)*DISTANCE_UNIT+1,(y()-1)*DISTANCE_UNIT +1 , DISTANCE_UNIT-1, DISTANCE_UNIT-1);
}
