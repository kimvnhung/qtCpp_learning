#include "griditem.h"

#include "graphitem.h"

#include <QPainter>

GridItem::GridItem(int displayRows, int displayColumns)
    : mDisplayRows{displayRows}
    , mDisplayColumns{displayColumns}
{

}

void GridItem::updateGrid(int displayRows, int displayColumns)
{
    mDisplayRows = displayRows;
    mDisplayColumns = displayColumns;
    update();
}

QRectF GridItem::boundingRect() const
{
    return QRectF(0, 0, mDisplayColumns*GraphItem::DISTANCE_UNIT, mDisplayRows*GraphItem::DISTANCE_UNIT);
}

void GridItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine));
    for (int i = 0; i <= mDisplayRows; ++i)
        painter->drawLine(0, i*GraphItem::DISTANCE_UNIT, mDisplayColumns*GraphItem::DISTANCE_UNIT, i*GraphItem::DISTANCE_UNIT);
    for (int i = 0; i <= mDisplayColumns; ++i)
        painter->drawLine(i*GraphItem::DISTANCE_UNIT, 0, i*GraphItem::DISTANCE_UNIT, mDisplayRows*GraphItem::DISTANCE_UNIT);
}

int GridItem::displayRows() const
{
    return mDisplayRows;
}

int GridItem::displayColumns() const
{
    return mDisplayColumns;
}


