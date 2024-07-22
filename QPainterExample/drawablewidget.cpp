#include "drawablewidget.h"

#include <QPainter>
#include <QPainterPath>

class DrawableWidget::Private{
public:
    Private(DrawableWidget* parent)
        :
        q(parent)
    {

    }

    DrawableWidget* q;

    enum DrawMode {
        None,
        Point,
        Line,
        Arc,
    };

    struct DrawPoint
    {
        DrawMode mode;
        QPointF point;
        qreal radius;
        bool clockwise;
    };

    QList<DrawPoint> drawPoints;
};

DrawableWidget::DrawableWidget(QWidget *parent)
    :
    QWidget(parent),
    d(new Private(this))
{

}

void DrawableWidget::moveTo(QPointF point)
{
    d->drawPoints.append({Private::Point, point, 0, false});
    update();
}

void DrawableWidget::lineTo(QPointF point)
{
    d->drawPoints.append({Private::Line, point, 0, false});
    update();
}

void DrawableWidget::arcTo(QPointF point, qreal radius, bool clockwise)
{
    d->drawPoints.append({Private::Arc, point, radius, clockwise});
    update();
}

QPainterPath DrawableWidget::path() const
{
    QPainterPath path;
    for (const auto& drawPoint : d->drawPoints) {
        switch (drawPoint.mode) {
        case Private::Line:
            path.lineTo(drawPoint.point);
            break;
        case Private::Arc:
            path.arcTo(QRectF(drawPoint.point.x() - drawPoint.radius, drawPoint.point.y() - drawPoint.radius, drawPoint.radius * 2, drawPoint.radius * 2), 0, 360);
            break;
        case Private::Point:
            path.moveTo(drawPoint.point);
            break;
        default:
            qDebug()<<"Unknown mode";
            break;
        }
    }
    return path;
}

void DrawableWidget::clear()
{
    d->drawPoints.clear();
    update();
}

void DrawableWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);

    painter.drawPath(path());
}

