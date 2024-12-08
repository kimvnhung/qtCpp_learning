#ifndef POLLUTANTOVERVIEWCHART_H
#define POLLUTANTOVERVIEWCHART_H

#include "chartholderbasewidget.h"

class PollutantOverviewChart : public ChartHolderBaseWidget

{
    Q_OBJECT
public:
    explicit PollutantOverviewChart(QWidget *parent = nullptr);

protected:
    void setUpChart() override;
};

#endif // POLLUTANTOVERVIEWCHART_H
