#ifndef COMPLIANCECHART_H
#define COMPLIANCECHART_H

#include "chartholderbasewidget.h"

#include <QPieSeries>



class ComplianceChart : public ChartHolderBaseWidget
{
    Q_OBJECT
public:
    explicit ComplianceChart(QWidget *parent = nullptr);

    QString summary() const override;
    QString name() const override;
public slots:
    void updateChart(int trueCount, int falseCount);

private:
    void setUpChart() override;

    QPieSeries *pieSeries;

};

#endif // COMPLIANCECHART_H
