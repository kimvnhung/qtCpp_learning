#include "rawdatapage.h"

#include "../common.h"

#include <QTableView>

RawDataPage::RawDataPage(QWidget *parent)
    : ChartHolderBaseWidget(parent)
    , m_waterModel(new WaterModel(this))
{
    LOG();
    initializeUi();
}

QString RawDataPage::summary() const
{
    LOG();
    return "Raw Data";
}

QString RawDataPage::name() const
{
    LOG();
    if(mode() == PREVIEW)
        return PREVIEW_MODE_NAME;
    else
        return RAW_DATA_PAGE;
}

void RawDataPage::setUpChart()
{
    LOG();
    //
    QTableView *table = new QTableView(this);
    table->setModel(m_waterModel);
    setChartWidget(table);
}

void RawDataPage::updateChart(std::vector<Water> data)
{
    LOG();
    m_waterModel->updateData(data);
}

