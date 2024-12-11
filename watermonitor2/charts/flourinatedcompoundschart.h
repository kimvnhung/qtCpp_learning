#ifndef FLOURINATEDCOMPOUNDSCHART_H
#define FLOURINATEDCOMPOUNDSCHART_H

#include "chartholderbasewidget.h"

class FlourinatedCompoundsChart : public ChartHolderBaseWidget
{
    Q_OBJECT
public:
    explicit FlourinatedCompoundsChart(QWidget *parent = nullptr);
    QString summary() const override;
    QString name() const override;

public slots:
    void updateChart(QStringList locations, QList<double> lats, QList<double> lons, QList<double> values, double minLat, double minLon, double maxLat, double maxLon, double maxValue);

protected:
    void setUpChart() override;
};

#endif // FLOURINATEDCOMPOUNDSCHART_H
