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

// Result will be a map, with key is the location, and value is a list of double values (revelant to materials list index)
void EnvironmentalLitterIndicatorsChart::updateChart(QStringList locations, QStringList materials, QMap<QString,QList<double>> results, double maxValue)
{
    LOGD(QString("locations: %1, materials: %2, results: %3, maxValue: %4").arg(locations.size()).arg(materials.size()).arg(results.size()).arg(maxValue));
    // Update the chart data
    QChart *chart = static_cast<QChartView *>(chartWidget())->chart();
    // Remove all series
    chart->removeAllSeries();
    // Remove all axes
    QList<QAbstractAxis *> axes = chart->axes();
    for (auto axis : axes)
    {
        chart->removeAxis(axis);
    }

    QList<QBarSet *> barSets;
    for (int i = 0; i < materials.size(); ++i)
    {
        QBarSet *barSet = new QBarSet(materials[i]);
        barSets.append(barSet);
    }

    for (int i = 0; i < locations.size(); ++i)
    {
        for (int j = 0; j < materials.size(); ++j)
        {
            *barSets[j] << results[locations[i]][j];
        }
    }

    QStackedBarSeries *stackedSeries = new QStackedBarSeries();
    for (auto barSet : barSets)
    {
        stackedSeries->append(barSet);
    }

    chart->addSeries(stackedSeries);

    // Set categories for X-axis
    QStringList categories = locations;
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    stackedSeries->attachAxis(axisX);

    // Set Y-axis
    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, maxValue*1.1);  // Adjust range as needed
    chart->addAxis(axisY, Qt::AlignLeft);
    stackedSeries->attachAxis(axisY);

    // Update the chart
    chart->update();
}
