#ifndef IMAGEPREVIEWWIDGET_H
#define IMAGEPREVIEWWIDGET_H

#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QWheelEvent>
#include <QMouseEvent>

class ImagePreviewWidget : public QGraphicsView {
    Q_OBJECT

public:
    explicit ImagePreviewWidget(QWidget *parent = nullptr);

    // Set the image path and load the image
    void setImagePath(const QString &path);

protected:
    // Override wheel event for zooming
    void wheelEvent(QWheelEvent *event) override;

    // Override mouse press, move, and release events for panning
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QGraphicsScene *m_scene;             // Graphics scene to hold the image
    QGraphicsPixmapItem *m_pixmapItem;   // Pixmap item to display the image
    bool m_isPanning;                    // Flag for panning state
    QPoint m_lastMousePosition;          // Last mouse position for panning
    double m_scaleFactor;                // Current zoom scale factor

    // Helper to reset view for a new image
    void resetView();
};

#endif // IMAGEPREVIEWWIDGET_H
