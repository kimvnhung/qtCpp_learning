#ifndef DRAWABLEWIDGET_H
#define DRAWABLEWIDGET_H

#include <QWidget>

class DrawableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DrawableWidget(QWidget *parent = nullptr);

    void moveTo(QPointF point);
    void lineTo(QPointF point);
    void arcTo(QPointF point, qreal radius, bool clockwise = false);
    void clear();

    QPainterPath path() const;

    void paintEvent(QPaintEvent* event) override;
private:
    class Private;
    QSharedPointer<Private> d;
};

#endif // DRAWABLEWIDGET_H
