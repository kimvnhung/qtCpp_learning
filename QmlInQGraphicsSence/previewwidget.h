#ifndef PREVIEWWIDGET_H
#define PREVIEWWIDGET_H

#include "graphitem.h"

#include <QGraphicsVideoItem>
#include <QMediaPlayer>

class PreviewWidget : public QGraphicsVideoItem, public GraphItem
{
    Q_OBJECT
public:
    PreviewWidget(int x, int y, QGraphicsItem *parent = nullptr);
    ~PreviewWidget();

    QRectF boundingRect() const override;
private:
    QMediaPlayer* m_player = NULL;
};

#endif // PREVIEWWIDGET_H
