#ifndef POPSCHART_H
#define POPSCHART_H

#include "chartholderbasewidget.h"

class POPsChart : public ChartHolderBaseWidget
{
    Q_OBJECT
public:
    explicit POPsChart(QWidget *parent = nullptr);

    QString summary() const override;
protected:
    void setUpChart() override;
};

#endif // POPSCHART_H
