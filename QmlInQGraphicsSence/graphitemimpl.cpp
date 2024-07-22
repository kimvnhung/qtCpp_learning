#include "graphitemimpl.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>

GraphItemImpl::GraphItemImpl(int x, int y, QObject *parent)
    : base_type{x,y,parent}
{
}

GraphItemImpl::~GraphItemImpl()
{
}

QRectF GraphItemImpl::boundingRect() const
{
    return GraphItem::boundingRect();
}

void GraphItemImpl::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(m_color);
    qDebug()<<"x:"<<GraphItem::x()<<"y:"<<GraphItem::y()<<"QRect: "<< boundingRect();
    painter->drawRect(boundingRect());
    if (m_isShowCordinate) {
        painter->drawText(boundingRect(), QString("%1,%2").arg(GraphItem::x()).arg(GraphItem::y()));
    }
}

void GraphItemImpl::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        emit rightMousePressed();
    }
    QGraphicsItem::mousePressEvent(event);
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
