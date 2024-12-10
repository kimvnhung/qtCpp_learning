#include "rawdatapage.h"

#include "../common.h"

RawDataPage::RawDataPage(QWidget *parent)
    : ChartHolderBaseWidget(parent)
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
}
