#include "environmentallitterindicatorschart.h"

#include "../common.h"

#include <QBarCategoryAxis>
#include <QBarSet>
#include <QChart>
#include <QChartView>
#include <QPieSeries>
#include <QStackedBarSeries>
#include <QValueAxis>

#define MAX_LOCATIONS_ALLOWED 10
#define MAX_MATERIALS_ALLOWED 20

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

    LOGD("Removed all series and axes");

    // Check if the number of locations and materials exceeds the maximum allowed
    if (locations.size() > MAX_LOCATIONS_ALLOWED)
    {
        LOGE("Number of locations or materials exceeds the maximum allowed");
        locations = locations.mid(0, MAX_LOCATIONS_ALLOWED);
    }

    if (materials.size() > MAX_MATERIALS_ALLOWED)
    {
        LOGE("Number of locations or materials exceeds the maximum allowed");
        materials = materials.mid(0, MAX_MATERIALS_ALLOWED);
    }

    // concat result if need
    QMap<QString, QList<double>> newResults;
    for (auto it = results.begin(); it != results.end(); ++it)
    {
        QString key = it.key();
        if (locations.contains(key))
        {
            auto values = it.value();
            values = values.mid(0, materials.size());
            newResults[key] = values;
        }
    }

    // Recalculate the maximum value
    if(locations.size() == MAX_LOCATIONS_ALLOWED || materials.size() == MAX_MATERIALS_ALLOWED)
    {
        maxValue = 0;
        for(auto location : locations)
        {
            double sum = 0;
            for (auto value : newResults[location])
            {
                sum += value;
            }

            if (sum > maxValue)
            {
                maxValue = sum;
            }
        }
    }

    QList<QBarSet *> barSets;
    for (int i = 0; i < materials.size(); ++i)
    {
        QBarSet *barSet = new QBarSet(materials[i]);
        barSets.append(barSet);
    }

    LOGD(QString("barSets: %1").arg(barSets.size()));
    for (int i = 0; i < locations.size(); ++i)
    {
        for (int j = 0; j < materials.size(); ++j)
        {
            *barSets[j] << newResults[locations[i]][j];
        }
    }

    LOGD(QString("barSets: %1").arg(barSets.size()));

    QStackedBarSeries *stackedSeries = new QStackedBarSeries();
    for (auto barSet : barSets)
    {
        stackedSeries->append(barSet);
    }

    LOGD("Added all barSets to stackedSeries");
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

    LOGD("Added all axes to chart");

    // Update the chart
    chart->update();
}

QString EnvironmentalLitterIndicatorsChart::summary() const
{
    return "Summary of Environmental Litter Indicators Chart";
}

QString EnvironmentalLitterIndicatorsChart::name() const
{
    if(mode() == PREVIEW)
    {
        return PREVIEW_MODE_NAME;
    }
    else
    {
        return ENVIRONMENT_LITTER_INDICATOR_CHART;
    }
}
