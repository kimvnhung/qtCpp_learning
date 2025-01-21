#include "imagepreviewwidget.h"
#include <QGraphicsScene>
#include <QPixmap>
#include <QDebug>
#include <QScrollBar>

ImagePreviewWidget::ImagePreviewWidget(QWidget *parent)
    : QGraphicsView(parent), m_scene(new QGraphicsScene(this)), m_pixmapItem(nullptr), m_isPanning(false), m_scaleFactor(1.0) {
    setScene(m_scene);
    setDragMode(QGraphicsView::NoDrag);  // Disable built-in dragging
    setRenderHint(QPainter::Antialiasing, true);
    setRenderHint(QPainter::SmoothPixmapTransform, true);
}

void ImagePreviewWidget::setImagePath(const QString &path) {
    QPixmap pixmap(path);
    if (pixmap.isNull()) {
        qDebug() << "Failed to load image:" << path;
        return;
    }

    if (m_pixmapItem) {
        m_scene->removeItem(m_pixmapItem);
        delete m_pixmapItem;
    }

    m_pixmapItem = m_scene->addPixmap(pixmap);
    resetView();
}

void ImagePreviewWidget::resetView() {
    m_scene->setSceneRect(m_pixmapItem->boundingRect());
    fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
    m_scaleFactor = 1.0;
}

void ImagePreviewWidget::wheelEvent(QWheelEvent *event) {
    const double scaleFactor = 1.15;
    if (event->angleDelta().y() > 0) {
        scale(scaleFactor, scaleFactor);  // Zoom in
        m_scaleFactor *= scaleFactor;
    } else {
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);  // Zoom out
        m_scaleFactor /= scaleFactor;
    }
}

void ImagePreviewWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_isPanning = true;
        m_lastMousePosition = event->pos();
        setCursor(Qt::ClosedHandCursor);
    }
    QGraphicsView::mousePressEvent(event);
}

void ImagePreviewWidget::mouseMoveEvent(QMouseEvent *event) {
    if (m_isPanning) {
        QPoint delta = event->pos() - m_lastMousePosition;
        m_lastMousePosition = event->pos();
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
    }
    QGraphicsView::mouseMoveEvent(event);
}

void ImagePreviewWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_isPanning = false;
        setCursor(Qt::ArrowCursor);
    }
    QGraphicsView::mouseReleaseEvent(event);
}
