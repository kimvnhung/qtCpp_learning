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
    // Create pie series
    pieSeries = new QPieSeries();
    pieSeries->append("TRUE", 0);
    pieSeries->append("FALSE", 0);

    // Customize slices
    QPieSlice *trueSlice = pieSeries->slices().at(0);
    trueSlice->setLabel("True: " + QString::number(0));
    trueSlice->setBrush(Qt::green);
    trueSlice->setExploded(); // Optional: highlight the slice

    QPieSlice *falseSlice = pieSeries->slices().at(1);
    falseSlice->setLabel("False: " + QString::number(0));
    falseSlice->setBrush(Qt::red);

    // Create chart
    QChart *chart = new QChart();
    chart->addSeries(pieSeries);
    chart->setTitle("Compliance Status (e.g., Compliant vs. Non-compliant)");
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    // Create a chart view
    auto chartView = new QChartView(chart);
    setChartWidget(chartView);
    chartView->setRenderHint(QPainter::Antialiasing); // Enable smooth rendering
}

QString ComplianceChart::summary() const
{
    return "This chart shows compliance data.";
}

QString ComplianceChart::name() const
{
    if (mode() == PREVIEW)
        return PREVIEW_MODE_NAME;
    else
        return COMPLIANCE_CHART;
}

void ComplianceChart::updateChart(int trueCount, int falseCount)
{
    LOGD(QString("True: %1, False: %2").arg(trueCount).arg(falseCount));
    // Update chart data
    QPieSlice *trueSlice = pieSeries->slices().at(0);
    QPieSlice *falseSlice = pieSeries->slices().at(1);

    trueSlice->setValue(trueCount);
    trueSlice->setLabel("True: " + QString::number(trueCount));

    falseSlice->setValue(falseCount);
    falseSlice->setLabel("False: " + QString::number(falseCount));

    double total = 0;
    for (QPieSlice *slice : pieSeries->slices()) {
        total += slice->value();
    }

    for (QPieSlice *slice : pieSeries->slices()) {
        double percentage = (slice->value() / total) * 100;
        slice->setLabel(slice->label().split(":")[0] + QString(": %1 (%2%)")
                                                           .arg(slice->value())
                                                           .arg(percentage, 0, 'f', 1));
    }
}
