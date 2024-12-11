#include "pollutantoverviewchart.h"
#include "../common.h"
#include <QChart>
#include <QChartView>
#include <QLineSeries>
#include <QGridLayout>
#include <QCategoryAxis>
#define MAX_POLLUTANTS 10
PollutantOverviewChart::PollutantOverviewChart(QWidget *parent)
    : ChartHolderBaseWidget{parent}
{
    LOG();
    initializeUi();
}
void PollutantOverviewChart::setUpChart()
{
    LOG();
    // Create a chart
    QChart *chart = new QChart();
    chart->setTitle("Pollutant Overview Chart");
    // Create multiple line series
    QLineSeries *series1 = new QLineSeries();
    series1->setName("Pollutant 1");
    series1->append(1, RAND_INT_MAX(100));
    series1->append(2, RAND_INT_MAX(100));
    series1->append(3, RAND_INT_MAX(100));
    series1->append(4, RAND_INT_MAX(100));
    series1->append(5, RAND_INT_MAX(100));
    series1->append(6, RAND_INT_MAX(100));
    series1->append(7, RAND_INT_MAX(100));
    series1->append(8, RAND_INT_MAX(100));
    series1->append(9, RAND_INT_MAX(100));
    series1->append(10, RAND_INT_MAX(100));
    series1->append(11, RAND_INT_MAX(100));
    series1->append(12, RAND_INT_MAX(100));
    QLineSeries *series2 = new QLineSeries();
    series2->setName("Pollutant 2");
    series2->append(1, RAND_INT_MAX(100));
    series2->append(2, RAND_INT_MAX(100));
    series2->append(3, RAND_INT_MAX(100));
    series2->append(4, RAND_INT_MAX(100));
    series2->append(5, RAND_INT_MAX(100));
    series2->append(6, RAND_INT_MAX(100));
    series2->append(7, RAND_INT_MAX(100));
    series2->append(8, RAND_INT_MAX(100));
    series2->append(9, RAND_INT_MAX(100));
    series2->append(10, RAND_INT_MAX(100));
    series2->append(11, RAND_INT_MAX(100));
    series2->append(12, RAND_INT_MAX(100));
    // Add series to the chart
    chart->addSeries(series1);
    chart->addSeries(series2);
    // Customize X-axis with month names
    QCategoryAxis *axisX = new QCategoryAxis();
    axisX->append("Jan", 1);
    axisX->append("Feb", 2);
    axisX->append("Mar", 3);
    axisX->append("Apr", 4);
    axisX->append("May", 5);
    axisX->append("Jun", 6);
    axisX->append("Jul", 7);
    axisX->append("Aug", 8);
    axisX->append("Sep", 9);
    axisX->append("Oct", 10);
    axisX->append("Nov", 11);
    axisX->append("Dec", 12);
    axisX->setTitleText("Months");
    axisX->setRange(0, 12); // Adjust range as needed
    // Customize Y-axis
    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Values");
    axisY->setRange(0, 110); // Adjust range as needed
    // Set axes on the chart
    // chart->addAxis(axisX, series1);
    // chart->addAxis(axisX, series2);
    // chart->addAxis(axisY, series1);
    // chart->addAxis(axisY, series2);
    chart->addAxis(axisY, Qt::AlignLeft);
    chart->addAxis(axisX, Qt::AlignBottom);
    series1->attachAxis(axisX);
    series1->attachAxis(axisY);
    series2->attachAxis(axisX);
    series2->attachAxis(axisY);
    // Create the chart view
    auto chartView = new QChartView(chart);
    setChartWidget(chartView);
    chartView->setRenderHint(QPainter::Antialiasing); // Enable smooth rendering
}
void PollutantOverviewChart::updateChart(QStringList pollutants, QList<QList<double>> valuesByMonth, double maxValue)
{
    LOG();
    // Update the chart here
    QChart *chart = static_cast<QChartView *>(chartWidget())->chart();
    // Remove all series
    chart->removeAllSeries();
    for (int i = 0; i < pollutants.size(); i++)
    {
        if (i >= MAX_POLLUTANTS)
            break;
        QLineSeries *series = new QLineSeries();
        series->setName(pollutants[i]);
        for (int j = 0; j < valuesByMonth.size(); j++)
        {
            series->append(j + 1, valuesByMonth[j][i]);
        }
        chart->addSeries(series);
    }
    // set range for Y-axis from chart
    for (auto axis : chart->axes(Qt::Vertical))
    {
        axis->setRange(0, maxValue * 1.2);
    }
}
QString PollutantOverviewChart::summary() const
{
    return "This chart provides an overview of pollutants.";
}
QString PollutantOverviewChart::name() const
{
    if (mode() == PREVIEW)
        return PREVIEW_MODE_NAME;
    else
        return POLLUTANT_OVERVIEW_CHART;
}
