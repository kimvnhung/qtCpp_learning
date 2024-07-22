#include "graphitem.h"

GraphItem::GraphItem(int x, int y, QObject *parent)
    : QObject{parent}
    , m_x{x}
    , m_y{y}
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
