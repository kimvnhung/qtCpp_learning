#include "previewgraphicsvideoitem.h"

PreviewGraphicsVideoItem::PreviewGraphicsVideoItem(int x, int y, QObject *parent)
    : base_type{NULL}
    , GraphItem{x,y}
{

}

PreviewGraphicsVideoItem::~PreviewGraphicsVideoItem()
{

}

QRectF PreviewGraphicsVideoItem::boundingRect() const
{
    QRectF rect = this->rect();
    QRectF result = QRectF(rect.center(),rect.size()*2);
    return result;
}
