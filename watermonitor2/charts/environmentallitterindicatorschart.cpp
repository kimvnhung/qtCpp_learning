#include "environmentallitterindicatorschart.h"

#include "../common.h"

#include <QChart>
#include <QChartView>
#include <QPieSeries>

EnvironmentalLitterIndicatorsChart::EnvironmentalLitterIndicatorsChart(QWidget *parent)
    : ChartHolderBaseWidget{parent}
{
    LOG();
    initializeUi();
}

void EnvironmentalLitterIndicatorsChart::setUpChart()
{
    LOG();
    // Create a pie series
    QPieSeries *series = new QPieSeries();
    series->append("Category A", 40); // Add slices
    series->append("Category B", 30);
    series->append("Category C", 20);
    series->append("Category D", 10);

    // Highlight a slice (optional)
    QPieSlice *slice = series->slices().at(1); // Second slice (Category B)
    slice->setExploded(true);
    slice->setLabelVisible(true);
    slice->setPen(QPen(Qt::darkGreen, 2));
    slice->setBrush(Qt::green);

    // Create a chart and add the pie series
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Environmental Litter Indicators");
    chart->legend()->setAlignment(Qt::AlignBottom);

    // Create a chart view
    setChartWidget(new QChartView(chart));
    chartWidget()->setRenderHint(QPainter::Antialiasing); // Enable smooth rendering
}
