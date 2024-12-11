#include "popschart.h"
#include "../common.h"
#include <QCategoryAxis>
#include <QChartView>
#include <QLineSeries>
#include <QPolarChart>
// #define MAX_LOCATIONS 10
POPsChart::POPsChart(QWidget *parent) : ChartHolderBaseWidget(parent)
{
    LOG();
    initializeUi();
}
void POPsChart::setUpChart()
{
    LOG();
    // Create a chart
    QChart *chart = new QChart();
    chart->setTitle("Persistent Organic Pollutants (POPs) Chart");
    // Create multiple line series
    QLineSeries *series1 = new QLineSeries();
    series1->setName("Location 1");
    series1->append(1, RAND_INT_MAX(30));
    series1->append(2, RAND_INT_MAX(30));
    series1->append(3, RAND_INT_MAX(30));
    series1->append(4, RAND_INT_MAX(30));
    series1->append(5, RAND_INT_MAX(30));
    series1->append(6, RAND_INT_MAX(30));
    series1->append(7, RAND_INT_MAX(30));
    series1->append(8, RAND_INT_MAX(30));
    series1->append(9, RAND_INT_MAX(30));
    series1->append(10, RAND_INT_MAX(30));
    series1->append(11, RAND_INT_MAX(30));
    series1->append(12, RAND_INT_MAX(30));
    QLineSeries *series2 = new QLineSeries();
    series2->setName("Location 2");
    series2->append(1, RAND_INT_MAX(30));
    series2->append(2, RAND_INT_MAX(30));
    series2->append(3, RAND_INT_MAX(30));
    series2->append(4, RAND_INT_MAX(30));
    series2->append(5, RAND_INT_MAX(30));
    series2->append(6, RAND_INT_MAX(30));
    series2->append(7, RAND_INT_MAX(30));
    series2->append(8, RAND_INT_MAX(30));
    series2->append(9, RAND_INT_MAX(30));
    series2->append(10, RAND_INT_MAX(30));
    series2->append(11, RAND_INT_MAX(30));
    series2->append(12, RAND_INT_MAX(30));
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
    axisY->setRange(0, 30 * 1.1); // Adjust range as needed
    // Set axes on the chart
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
QString POPsChart::summary() const
{
    return "This chart displays Persistent Organic Pollutants (POPs) data.";
}
QString POPsChart::name() const
{
    if (mode() == PREVIEW)
        return PREVIEW_MODE_NAME;
    else
        return POPS_CHART;
}
void POPsChart::updateChart(QStringList locations, QList<QList<double>> valuesByMonth, double maxValue)
{
    LOG();
    // Update the chart here
    QChart *chart = static_cast<QChartView *>(chartWidget())->chart();
    // Remove all series
    chart->removeAllSeries();
    for (int i = 0; i < locations.size(); i++)
    {
        // if (i >= MAX_LOCATIONS)
        //     break;
        QLineSeries *series = new QLineSeries();
        series->setName(locations[i]);
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
