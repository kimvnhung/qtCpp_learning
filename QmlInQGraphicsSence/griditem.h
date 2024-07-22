#ifndef GRIDITEM_H
#define GRIDITEM_H

#include <QGraphicsItem>

class GridItem : public QGraphicsItem
{
public:
    GridItem(int displayRows = 1,int displayColumns = 1);

    void updateGrid(int displayRows, int displayColumns);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    int displayRows() const;
    int displayColumns() const;
private:
    int mDisplayRows;
    int mDisplayColumns;
};

#endif // GRIDITEM_H
