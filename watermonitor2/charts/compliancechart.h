#ifndef COMPLIANCECHART_H
#define COMPLIANCECHART_H

#include "chartholderbasewidget.h"



class ComplianceChart : public ChartHolderBaseWidget
{
    Q_OBJECT
public:
    explicit ComplianceChart(QWidget *parent = nullptr);

    QString summary() const override;
    QString name() const override;
private:
    void setUpChart() override;
};

#endif // COMPLIANCECHART_H
