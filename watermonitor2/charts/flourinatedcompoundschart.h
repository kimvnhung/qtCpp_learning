#ifndef FLOURINATEDCOMPOUNDSCHART_H
#define FLOURINATEDCOMPOUNDSCHART_H

#include "chartholderbasewidget.h"

class FlourinatedCompoundsChart : public ChartHolderBaseWidget
{
    Q_OBJECT
public:
    explicit FlourinatedCompoundsChart(QWidget *parent = nullptr);

protected:
    void setUpChart() override;
};

#endif // FLOURINATEDCOMPOUNDSCHART_H
