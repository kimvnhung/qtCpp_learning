#include "graphitemimpl.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>

GraphItemImpl::GraphItemImpl(int x, int y, QObject *parent)
    : base_type{x,y,parent}
{
    setAcceptHoverEvents(true);
}

GraphItemImpl::~GraphItemImpl()
{
}

QRectF GraphItemImpl::boundingRect() const
{
    return rect();
}

void GraphItemImpl::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(m_color);
    painter->drawRect(boundingRect());
    if (m_isShowCordinate) {
        painter->drawText(boundingRect(), QString("%1,%2").arg(xUnit()).arg(yUnit()));
    }
}

void GraphItemImpl::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        emit rightMousePressed();
    }
    QGraphicsItem::mousePressEvent(event);
}

void GraphItemImpl::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    qDebug()<<__FUNCTION__;
    emit hoverEnter();
    QGraphicsItem::hoverEnterEvent(event);
}

void GraphItemImpl::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    qDebug()<<__FUNCTION__;
    emit hoverLeave();
    QGraphicsItem::hoverLeaveEvent(event);
}

void GraphItemImpl::changeColor()
{
    m_color = QColor(rand()%256, rand()%256, rand()%256);
    update();
}

void GraphItemImpl::switchShowCordinate()
{
    m_isShowCordinate = !m_isShowCordinate;
    update();
}
