#ifndef CHARTHOLDERBASEWIDGET_H
#define CHARTHOLDERBASEWIDGET_H

#include <QWidget>
#include <QChartView>

class ChartHolderBaseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChartHolderBaseWidget(QWidget *parent = nullptr);

private:
    QChartView* m_chartContent = nullptr;
};

#endif // CHARTHOLDERBASEWIDGET_H
