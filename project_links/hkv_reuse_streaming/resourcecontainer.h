#ifndef RESOURCECONTAINER_H
#define RESOURCECONTAINER_H

#include <QGraphicsItem>
#include <QRandomGenerator>
class ResourceContainer : public QGraphicsItem
{
public:
    ResourceContainer();


    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;


protected:
    void advance(int step) override;

private:
    qreal angle = 0;
    qreal speed = 0;
    qreal mouseEyeDirection = 0;
    QColor color;
};

#endif // RESOURCECONTAINER_H
