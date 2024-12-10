#ifndef POLLUTANTOVERVIEWCHART_H
#define POLLUTANTOVERVIEWCHART_H

#include "chartholderbasewidget.h"

class PollutantOverviewChart : public ChartHolderBaseWidget

{
    Q_OBJECT
public:
    explicit PollutantOverviewChart(QWidget *parent = nullptr);

    QString summary() const override;
    QString name() const override;
public slots:
    void updateChart(QStringList pollutants, QList<QList<double>> valuesByMonth, double maxValue);

protected:
    void setUpChart() override;
};

#endif // POLLUTANTOVERVIEWCHART_H
