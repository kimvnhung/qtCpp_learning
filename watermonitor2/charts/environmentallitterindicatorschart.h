#ifndef ENVIRONMENTALLITTERINDICATORSCHART_H
#define ENVIRONMENTALLITTERINDICATORSCHART_H

#include "chartholderbasewidget.h"

class EnvironmentalLitterIndicatorsChart : public ChartHolderBaseWidget
{
    Q_OBJECT
public:
    explicit EnvironmentalLitterIndicatorsChart(QWidget *parent = nullptr);

public slots:
    void updateChart();

protected:
    void setUpChart() override;
};

#endif // ENVIRONMENTALLITTERINDICATORSCHART_H
