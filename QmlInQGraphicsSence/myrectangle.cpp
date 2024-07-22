#include "myrectangle.h"

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QPainter>

MyRectangle::MyRectangle()
    : mPressed(false)
{
    setFlag(ItemIsMovable);
}


QRectF MyRectangle::boundingRect() const
{
    return QRectF(0, 0, 100, 100);
}

void MyRectangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    if (mPressed)
        painter->setBrush(Qt::red);
    else
        painter->setBrush(Qt::green);
    painter->drawRect(0, 0, 100, 100);
}

bool MyRectangle::pressed() const
{
    return mPressed;
}

void MyRectangle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // qDebug()<<__FUNCTION__<<__LINE__;
    mPressed = true;
    update();
    QGraphicsItem::mousePressEvent(event);
}

void MyRectangle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    // qDebug()<<"mouseRelease";
    mPressed = false;
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
