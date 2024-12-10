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
    series1->setName("Series 1");
    series1->append(1, 10);
    series1->append(2, 20);
    series1->append(3, 15);

    QLineSeries *series2 = new QLineSeries();
    series2->setName("Series 2");
    series2->append(1, 5);
    series2->append(2, 15);
    series2->append(3, 25);

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
    axisY->setRange(0, 100); // Adjust range as needed

    // Set axes on the chart
    chart->setAxisX(axisX, series1);
    chart->setAxisX(axisX, series2);
    chart->setAxisY(axisY, series1);
    chart->setAxisY(axisY, series2);

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

    for(int i = 0; i < pollutants.size(); i++)
    {
        if(i >= MAX_POLLUTANTS)
            break;

        QLineSeries *series = new QLineSeries();
        series->setName(pollutants[i]);
        for(int j = 0; j < valuesByMonth.size(); j++)
        {
            series->append(j+1, valuesByMonth[j][i]);
        }
        chart->addSeries(series);
    }

    // set range for Y-axis from chart
    chart->axisY()->setRange(0, maxValue*1.2);
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
