#ifndef CHARTHOLDERBASEWIDGET_H
#define CHARTHOLDERBASEWIDGET_H

#include <QWidget>

class ChartHolderBaseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChartHolderBaseWidget(QWidget *parent = nullptr);
    ~ChartHolderBaseWidget() = default;

};

#endif // CHARTHOLDERBASEWIDGET_H
