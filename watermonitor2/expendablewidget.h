#ifndef EXPENDABLEWIDGET_H
#define EXPENDABLEWIDGET_H

#include <QWidget>

class ExpandableWidget : public QWidget {
    Q_OBJECT

public:
    explicit ExpandableWidget(QWidget *parent = nullptr);

protected:
    void enterEvent(QEnterEvent *event) override;    // Mouse enters widget
    void leaveEvent(QEvent *event) override;    // Mouse leaves widget
    void paintEvent(QPaintEvent *event) override; // To paint the widget

private:
    bool m_hovered; // Boolean to check if the widget is hovered
};

#endif // EXPENDABLEWIDGET_H
