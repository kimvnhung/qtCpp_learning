#ifndef RAWDATAPAGE_H
#define RAWDATAPAGE_H

#include "chartholderbasewidget.h"
#include "../handlers/model.hpp"

class RawDataPage : public ChartHolderBaseWidget
{
    Q_OBJECT
public:
    explicit RawDataPage(QWidget *parent = nullptr);

    QString summary() const override;
    QString name() const override;
public slots:
    void updateChart(std::vector<Water> data);

protected:
    void setUpChart() override;
private:
    WaterModel *m_waterModel = nullptr;
};

#endif // RAWDATAPAGE_H
