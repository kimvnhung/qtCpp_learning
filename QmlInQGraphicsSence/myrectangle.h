#ifndef MYRECTANGLE_H
#define MYRECTANGLE_H

#include <QGraphicsItem>

class MyRectangle : public QGraphicsItem
{
public:
    MyRectangle();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    bool pressed() const;
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    bool mPressed;
};

#endif // MYRECTANGLE_H
