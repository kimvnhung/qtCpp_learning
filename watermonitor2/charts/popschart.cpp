#include "popschart.h"

#include "../common.h"

#include <QCategoryAxis>
#include <QChartView>
#include <QLineSeries>
#include <QPolarChart>

POPsChart::POPsChart(QWidget *parent)
    : ChartHolderBaseWidget(parent)
{
    LOG();
    initializeUi();
}

void POPsChart::setUpChart()
{
    LOG();
    // Create a polar chart
    QPolarChart *chart = new QPolarChart();
    chart->setTitle("Persistent Organic Pollutants (POPs)");

    // Create categories (e.g., axes for metrics)
    QStringList categories = {"Metric 1", "Metric 2", "Metric 3", "Metric 4", "Metric 5"};

    // Create a line series for data points
    QLineSeries *series = new QLineSeries();
    *series << QPointF(0, 5) << QPointF(1, 3) << QPointF(2, 4) << QPointF(3, 2) << QPointF(4, 4) << QPointF(0, 5);

    // Add the series to the chart
    chart->addSeries(series);

    // Configure angular axis (categories)
    QCategoryAxis *angularAxis = new QCategoryAxis();
    for (int i = 0; i < categories.size(); ++i) {
        angularAxis->append(categories[i], i);
    }
    angularAxis->setRange(0, categories.size());
    angularAxis->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);
    chart->addAxis(angularAxis, QPolarChart::PolarOrientationAngular);
    series->attachAxis(angularAxis);

    // Configure radial axis (values)
    QValueAxis *radialAxis = new QValueAxis();
    radialAxis->setRange(0, 5); // Adjust range based on your data
    radialAxis->setTickCount(6);
    radialAxis->setLabelFormat("%.0f");
    chart->addAxis(radialAxis, QPolarChart::PolarOrientationRadial);
    series->attachAxis(radialAxis);

    // Create a chart view
    setChartWidget(new QChartView(chart));
    chartWidget()->setRenderHint(QPainter::Antialiasing); // Enable smooth rendering
}
