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
    QBarSet *material1 = new QBarSet("Platics");
    QBarSet *material2 = new QBarSet("Organics");
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
    chart->setTitle("Environmental Litter Indicators");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Set categories for X-axis (e.g., times)
    QStringList categories = {"NewYour", "Paris", "Somewhere"};
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

void EnvironmentalLitterIndicatorsChart::updateChart()
{
    LOG();
    // Update the chart data
    QChart *chart = static_cast<QChartView *>(chartWidget())->chart();
    QStackedBarSeries *stackedSeries = static_cast<QStackedBarSeries *>(chart->series().at(0));

    // Update the data for each material
    QBarSet *material1 = stackedSeries->barSets().at(0);
    QBarSet *material2 = stackedSeries->barSets().at(1);
    QBarSet *material3 = stackedSeries->barSets().at(2);

    // Update the data for locations and times
    *material1 << 40 << 50 << 60;  // New data for time 1, 2, 3
    *material2 << 45 << 55 << 65;
    *material3 << 35 << 45 << 55;

    // Update the chart
    chart->update();
}
