#ifndef POPSCHART_H
#define POPSCHART_H
#include "chartholderbasewidget.h"
class POPsChart : public ChartHolderBaseWidget
{
    Q_OBJECT
public:
    explicit POPsChart(QWidget *parent = nullptr);
    QString summary() const override;
    QString name() const override;
public slots:
    void updateChart(QStringList locations, QList<QList<double>> valuesByMonth, double maxValue);

protected:
    void setUpChart() override;
};
#endif // POPSCHART_H
