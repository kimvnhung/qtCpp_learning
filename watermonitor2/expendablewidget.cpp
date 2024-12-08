#include "expendablewidget.h"

#include <QPainter>
#include <QTimer>

ExpandableWidget::ExpandableWidget(QWidget *parent)
    : QWidget(parent), m_hovered(false) {
    setFixedSize(100, 100); // Initial size
}

void ExpandableWidget::enterEvent(QEnterEvent *event) {
    // When the mouse enters, expand the widget
    m_hovered = true;
    setFixedSize(150, 150); // Expanded size
    update(); // Redraw the widget
    raise(); // Bring the widget to the top (Z-order)
    QWidget::enterEvent(event);
}

void ExpandableWidget::leaveEvent(QEvent *event) {
    // When the mouse leaves, revert the size
    m_hovered = false;
    setFixedSize(100, 100); // Original size
    update();
    QWidget::leaveEvent(event);
}

void ExpandableWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setPen(Qt::black);
    painter.setBrush(m_hovered ? Qt::yellow : Qt::lightGray); // Color change on hover
    painter.drawRect(0, 0, width(), height());
    QWidget::paintEvent(event);
}
