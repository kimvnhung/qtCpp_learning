#include "compliancechart.h"

#include "../common.h"

#include <QBarCategoryAxis>
#include <QBarSeries>
#include <QBarSet>
#include <QChart>
#include <QChartView>
#include <QValueAxis>

ComplianceChart::ComplianceChart(QWidget *parent)
    : ChartHolderBaseWidget{parent}
{
    LOG();
    initializeUi();
}

void ComplianceChart::setUpChart()
{
    LOG();
    // Create bar sets
    QBarSet *set0 = new QBarSet("Series 1");
    QBarSet *set1 = new QBarSet("Series 2");

    // Add data to bar sets
    *set0 << 1 << 2 << 3 << 4 << 5;
    *set1 << 5 << 0 << 0 << 4 << 1;

    // Create a bar series and append sets
    QBarSeries *series = new QBarSeries();
    series->append(set0);
    series->append(set1);

    // Create a chart
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Compliance Chart");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Create category axis for X-axis
    QStringList categories;
    categories << "Jan" << "Feb" << "Mar" << "Apr" << "May";
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Create value axis for Y-axis
    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 6);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // Create a chart view
    setChartWidget(new QChartView(chart));
    chartWidget()->setRenderHint(QPainter::Antialiasing); // Enable smooth rendering
}

QString ComplianceChart::summary() const
{
    return "This chart shows compliance data.";
}
