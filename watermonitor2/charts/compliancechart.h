#ifndef COMPLIANCECHART_H
#define COMPLIANCECHART_H

#include "chartholderbasewidget.h"



class ComplianceChart : public ChartHolderBaseWidget
{
    Q_OBJECT
public:
    explicit ComplianceChart(QWidget *parent = nullptr);

private:
    void setUpChart() override;
};

#endif // COMPLIANCECHART_H
