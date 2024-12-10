#include "pollutantoverviewchart.h"

#include "../common.h"

#include <QChart>
#include <QChartView>
#include <QLineSeries>
#include <QGridLayout>

PollutantOverviewChart::PollutantOverviewChart(QWidget *parent)
    : ChartHolderBaseWidget{parent}
{
    LOG();
    initializeUi();
}

void PollutantOverviewChart::setUpChart()
{
    LOG();
    // Create a new line series
    QLineSeries *series = new QLineSeries();
    series->append(0, 0);
    series->append(1, 2);
    series->append(2, 3);
    series->append(3, 5);
    series->append(4, 8);

    // Create the chart and add the series
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Pollutant Overview");
    chart->createDefaultAxes();

    // Create the chart view
    auto chartView = new QChartView(chart);
    setChartWidget(chartView);
    chartView->setRenderHint(QPainter::Antialiasing); // Enable smooth rendering
}

QString PollutantOverviewChart::summary() const
{
    return "This chart provides an overview of pollutants.";
}

QString PollutantOverviewChart::name() const
{
    if(mode() == PREVIEW)
        return PREVIEW_MODE_NAME;
    else
        return POLLUTANT_OVERVIEW_CHART;
}
