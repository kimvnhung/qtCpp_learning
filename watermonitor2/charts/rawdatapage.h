#ifndef RAWDATAPAGE_H
#define RAWDATAPAGE_H

#include "chartholderbasewidget.h"

class RawDataPage : public ChartHolderBaseWidget
{
    Q_OBJECT
public:
    explicit RawDataPage(QWidget *parent = nullptr);

    QString summary() const override;
    QString name() const override;
protected:
    void setUpChart() override;
};

#endif // RAWDATAPAGE_H
