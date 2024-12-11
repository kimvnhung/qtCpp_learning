#include "flourinatedcompoundschart.h"
#include "../common.h"
#include <QBarCategoryAxis>
#include <QBarSeries>
#include <QBarSet>
#include <QChart>
#include <QChartView>
#include <QScatterSeries>
#include <QValueAxis>
#define MAX_BUBBLE_SIZE 25

FlourinatedCompoundsChart::FlourinatedCompoundsChart(QWidget *parent)
    : ChartHolderBaseWidget{parent}
{
    LOG();
    initializeUi();
}

void FlourinatedCompoundsChart::setUpChart()
{
    LOG();
    // Create a scatter series (bubble chart series)
    int numLocations = 6;
    QList<QScatterSeries *> series;
    for (int i = 0; i < numLocations; i++)
    {
        auto bubble = new QScatterSeries(this);
        bubble->setName("Location 1");
        bubble->setMarkerShape(QScatterSeries::MarkerShapeCircle);
        double pollutantValue = RANDOM_DOUBLE_MAX(MAX_BUBBLE_SIZE);
        // if(pollutantValue/MAX_BUBBLE_SIZE <= 0.33)
        //     bubble->setColor(LOW_LEVEL_COLOR);
        // else if (pollutantValue/MAX_BUBBLE_SIZE <= 0.66)
        //     bubble->setColor(MID_LEVEL_COLOR);
        // else
        //     bubble->setColor(HIGH_LEVEL_COLOR);
        bubble->setMarkerSize(pollutantValue);
        bubble->append(RANDOM_DOUBLE_MAX(50), RANDOM_DOUBLE_MAX(50));
        series.append(bubble);
    }
    // Create a chart and add the series
    QChart *chart = new QChart();
    for (auto ser : series)
    {
        chart->addSeries(ser);
    }
    chart->setTitle("Bubble Chart Example");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    // Create axes and set them to the chart
    QValueAxis *axisX = new QValueAxis();
    axisX->setTitleText("X Axis");
    axisX->setLabelFormat("%i");
    axisX->setRange(0, 50);
    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Y Axis");
    axisY->setLabelFormat("%i");
    axisY->setRange(0, 50);
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    for (auto ser : series)
    {
        ser->attachAxis(axisX);
        ser->attachAxis(axisY);
    }
    // Create a chart view
    auto chartView = new QChartView(chart);
    setChartWidget(chartView);
    chartView->setRenderHint(QPainter::Antialiasing); // Enable smooth rendering
}

QString FlourinatedCompoundsChart::summary() const
{
    return "This chart displays the frequency of flourinated compounds in different bins.";
}

QString FlourinatedCompoundsChart::name() const
{
    if (mode() == PREVIEW)
        return PREVIEW_MODE_NAME;
    else
        return FLOURINATED_COMPOUNDS_CHART;
}

void FlourinatedCompoundsChart::updateChart(
    QStringList locations, QList<double> lats, QList<double> lons,
    QList<double> values, double minLat, double minLon,
    double maxLat, double maxLon, double maxValue)
{
    LOG();
    auto chart = static_cast<QChartView *>(chartWidget())->chart();
    auto axisXs = chart->axes(Qt::Horizontal);
    auto axisYs = chart->axes(Qt::Vertical);
    chart->removeAllSeries();
    QList<QScatterSeries *> series;
    for (int i = 0; i < locations.size(); i++)
    {
        auto bubble = new QScatterSeries(this);
        bubble->setName(locations[i]);
        bubble->setMarkerShape(QScatterSeries::MarkerShapeCircle);
        double maxBubbleSize = std::min(maxLat - minLat, maxLon - minLon);
        double pollutantValue = maxBubbleSize * values[i] / maxValue;
        // if(pollutantValue/MAX_BUBBLE_SIZE <= 0.33)
        //     bubble->setColor(LOW_LEVEL_COLOR);
        // else if (pollutantValue/MAX_BUBBLE_SIZE <= 0.66)
        //     bubble->setColor(MID_LEVEL_COLOR);
        // else
        //     bubble->setColor(HIGH_LEVEL_COLOR);
        bubble->setMarkerSize(pollutantValue);
        bubble->append(lats[i], lons[i]);
        series.append(bubble);
    }
    for (auto ser : series)
    {
        chart->addSeries(ser);
    }
    for (auto axisX : axisXs)
    {
        axisX->setRange(minLat, maxLat);
        for (auto ser : series)
        {
            ser->attachAxis(axisX);
        }
    }
    for (auto axisY : axisYs)
    {
        axisY->setRange(minLon, maxLon);
        for (auto ser : series)
        {
            ser->attachAxis(axisY);
        }
    }
}
