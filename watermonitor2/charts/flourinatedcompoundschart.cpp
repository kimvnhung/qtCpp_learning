#include "flourinatedcompoundschart.h"
#include "../common.h"
#include <QBarCategoryAxis>
#include <QBarSeries>
#include <QBarSet>
#include <QChart>
#include <QChartView>
#include <QScatterSeries>
#include <QValueAxis>
#define MAX_BUBBLE_SIZE 100
#define MAX_LOCATIONS 20

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
    chart->setTitle("Flourinated Compounds Chart");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    // Create axes and set them to the chart
    QValueAxis *axisX = new QValueAxis();
    axisX->setTitleText("Latitude");
    axisX->setLabelFormat("%i");
    axisX->setRange(0, 50);
    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Longtitude");
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
    LOGD(QString("locations.size() %1, lats.size() %2, lons.size() %3, values.size() %4, minLat %5, minLon %6, maxLat %7, maxLon %8, maxValue %9")
             .arg(locations.size())
             .arg(lats.size())
             .arg(lons.size())
             .arg(values.size())
             .arg(minLat)
             .arg(minLon)
             .arg(maxLat)
             .arg(maxLon)
             .arg(maxValue));



    auto chart = static_cast<QChartView *>(chartWidget())->chart();
    auto axisXs = chart->axes(Qt::Horizontal);
    auto axisYs = chart->axes(Qt::Vertical);
    chart->removeAllSeries();
    QList<QScatterSeries *> series;
    for (int i = 0; i < locations.size(); i++)
    {
        if(i >= MAX_LOCATIONS)
            break;

        auto bubble = new QScatterSeries(this);
        bubble->setName(locations[i]);
        bubble->setMarkerShape(QScatterSeries::MarkerShapeCircle);
        double pollutantValue = MAX_BUBBLE_SIZE * values[i]*0.5 / maxValue;
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
        axisX->setRange(minLat*0.9, maxLat*1.1);
        for (auto ser : series)
        {
            ser->attachAxis(axisX);
        }
    }
    for (auto axisY : axisYs)
    {
        axisY->setRange(minLon*0.9, maxLon*1.1);
        for (auto ser : series)
        {
            ser->attachAxis(axisY);
        }
    }
}
