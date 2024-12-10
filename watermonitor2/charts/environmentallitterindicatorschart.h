#ifndef ENVIRONMENTALLITTERINDICATORSCHART_H
#define ENVIRONMENTALLITTERINDICATORSCHART_H

#include "chartholderbasewidget.h"

class EnvironmentalLitterIndicatorsChart : public ChartHolderBaseWidget
{
    Q_OBJECT
public:
    explicit EnvironmentalLitterIndicatorsChart(QWidget *parent = nullptr);

    QString summary() const override;
    QString name() const override;
public slots:
    void updateChart(QStringList locations, QStringList materials, QMap<QString,QList<double>> results, double maxValue);

protected:
    void setUpChart() override;
};

#endif // ENVIRONMENTALLITTERINDICATORSCHART_H
