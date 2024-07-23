#ifndef PREVIEWGRAPHICSVIDEOITEM_H
#define PREVIEWGRAPHICSVIDEOITEM_H

#include "graphitem.h"

#include <QGraphicsVideoItem>
#include <QObject>

class PreviewGraphicsVideoItem : public QGraphicsVideoItem, public GraphItem
{
    Q_OBJECT
    using base_type = QGraphicsVideoItem;
public:
    PreviewGraphicsVideoItem(int x = 0, int y = 0, QObject *parent = nullptr);
    ~PreviewGraphicsVideoItem() override;

    QRectF boundingRect() const override;
};

#endif // PREVIEWGRAPHICSVIDEOITEM_H
