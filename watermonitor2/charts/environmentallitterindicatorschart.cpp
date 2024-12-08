#include "environmentallitterindicatorschart.h"

#include "../common.h"

#include <QBarCategoryAxis>
#include <QBarSet>
#include <QChart>
#include <QChartView>
#include <QPieSeries>
#include <QStackedBarSeries>
#include <QValueAxis>

EnvironmentalLitterIndicatorsChart::EnvironmentalLitterIndicatorsChart(QWidget *parent)
    : ChartHolderBaseWidget{parent}
{
    LOG();
    initializeUi();
}

void EnvironmentalLitterIndicatorsChart::setUpChart()
{
    LOG();
    // Create bar sets for materials
    QBarSet *material1 = new QBarSet("Material 1");
    QBarSet *material2 = new QBarSet("Material 2");
    QBarSet *material3 = new QBarSet("Material 3");

    // Add data for locations and times
    *material1 << 10 << 20 << 30;  // Data for time 1, 2, 3
    *material2 << 15 << 25 << 35;
    *material3 << 5 << 15 << 25;

    // Create a stacked bar series
    QStackedBarSeries *stackedSeries = new QStackedBarSeries();
    stackedSeries->append(material1);
    stackedSeries->append(material2);
    stackedSeries->append(material3);

    // Create the chart
    QChart *chart = new QChart();
    chart->addSeries(stackedSeries);
    chart->setTitle("Stacked Bar Chart");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Set categories for X-axis (e.g., times)
    QStringList categories = {"Time 1", "Time 2", "Time 3"};
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    stackedSeries->attachAxis(axisX);

    // Set Y-axis
    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 100);  // Adjust range as needed
    chart->addAxis(axisY, Qt::AlignLeft);
    stackedSeries->attachAxis(axisY);

    // Create a chart view
    setChartWidget(new QChartView(chart));
    chartWidget()->setRenderHint(QPainter::Antialiasing); // Enable smooth rendering
}
