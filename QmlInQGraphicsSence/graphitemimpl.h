#ifndef GRAPHITEMIMPL_H
#define GRAPHITEMIMPL_H

#include "graphitem.h"

#include <QGraphicsItem>

class GraphItemImpl : public GraphItem, public QGraphicsItem
{
    Q_OBJECT
    using base_type = GraphItem;
public:
    explicit GraphItemImpl(int x = 0, int y = 0, QObject *parent = nullptr);
    ~GraphItemImpl() override;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void changeColor();
    void switchShowCordinate();
signals:
    void rightMousePressed();
    void hoverEnter();
    void hoverLeave();
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
private:
    QColor m_color;
    bool m_isShowCordinate;

};

#endif // GRAPHITEMIMPL_H
