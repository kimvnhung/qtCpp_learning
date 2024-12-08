#include "flourinatedcompoundschart.h"

#include "../common.h"

#include <QBarCategoryAxis>
#include <QBarSeries>
#include <QBarSet>
#include <QChart>
#include <QChartView>
#include <QValueAxis>

FlourinatedCompoundsChart::FlourinatedCompoundsChart(QWidget *parent)
    : ChartHolderBaseWidget{parent}
{
    LOG();
    initializeUi();
}

void FlourinatedCompoundsChart::setUpChart()
{
    LOG();
    // Define the bins and their frequencies
    QVector<int> bins = {1, 2, 3, 4, 5};
    QVector<int> frequencies = {5, 15, 8, 20, 10};

    // Create a bar set and add data
    QBarSet *set = new QBarSet("Frequency");
    for (int frequency : frequencies) {
        *set << frequency;
    }

    // Create a bar series
    QBarSeries *series = new QBarSeries();
    series->append(set);

    // Create a chart and add the series
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Flourinated Compounds Chart");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Create category axis for bins
    QStringList categories;
    for (int bin : bins) {
        categories << QString::number(bin);
    }

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Create a value axis for frequencies
    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 25); // Adjust based on data range
    axisY->setTitleText("Frequency");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // Create a chart view
    setChartWidget(new QChartView(chart));
    chartWidget()->setRenderHint(QPainter::Antialiasing); // Enable smooth rendering
}
