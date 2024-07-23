#include "customgraphicsview.h"

#include <QWheelEvent>
#include <QScrollBar>
#include <QMenu>

CustomGraphicsView::CustomGraphicsView(QWidget *parent)
    : base_type{parent}
{}

CustomGraphicsView::CustomGraphicsView(QGraphicsScene *scene, QWidget *parent)
    : base_type{scene, parent}
{

}

void CustomGraphicsView::wheelEvent(QWheelEvent *event)
{
    switch (event->modifiers()) {
    case Qt::ControlModifier:
        if (event->angleDelta().x() != 0)
            QAbstractScrollArea::horizontalScrollBar()->setValue(QAbstractScrollArea::horizontalScrollBar()->value() - (event->angleDelta().y()));
        else
            QAbstractScrollArea::verticalScrollBar()->setValue(QAbstractScrollArea::verticalScrollBar()->value() - (event->angleDelta().y()));
        break;
    case Qt::ShiftModifier:
        QAbstractScrollArea::horizontalScrollBar()->setValue(QAbstractScrollArea::horizontalScrollBar()->value() - (event->angleDelta().y()));
        break;
    case Qt::NoModifier:
        if (abs(event->angleDelta().y()) == 120) {
            if (event->angleDelta().y() > 0)
                zoomIn();
            else
                zoomOut();
        }
        break;
    default:
        QGraphicsView::wheelEvent(event);
        return;
    }
    event->accept();
}

const double zoomFactor = 1.3;

void CustomGraphicsView::zoomIn()
{
    scale(zoomFactor, zoomFactor);
}

void CustomGraphicsView::zoomOut()
{
    scale(1.0 / zoomFactor, 1.0 / zoomFactor);
}

void CustomGraphicsView::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    // fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
}

void CustomGraphicsView::contextMenuEvent(QContextMenuEvent *event)
{
    // QMenu menu(this);
    // menu.addAction(QAction("Zoom In", this),);

    QGraphicsView::contextMenuEvent(event);
}
