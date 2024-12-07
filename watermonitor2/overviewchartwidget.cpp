#include "overviewchartwidget.h"
#include "charts/chartholderbasewidget.h"

#include <QGridLayout>
#include <QLabel>

OverviewChartWidget::OverviewChartWidget(QWidget *parent)
    : QWidget{parent}
{
    initializeUi();
}

//private
void OverviewChartWidget::initializeUi()
{
    // Set for always expanding
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_mainLayout = new QGridLayout;

    setUpChart();

    setLayout(m_mainLayout);
}

void OverviewChartWidget::setUpChart()
{
    m_listChart = new QList<ChartWidget*>;

    for (int i = 0; i < 3; ++i) {
        auto chart = new ChartWidget(this);
        m_listChart->append(chart);
        m_mainLayout->addWidget(chart, i, 0);
    }
}
