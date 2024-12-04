#include "pollutantOverviewPage.hpp"
#include "ui_elements.hpp"
#include <QBarCategoryAxis>
#include <QBarSeries>
#include <QBarSet>
#include <QChartView>
#include <QValueAxis>
#include <QtWidgets>

#include <algorithm>
#include "common.hpp"


PollutantOverviewPage::PollutantOverviewPage(QWidget *parent)
    : QWidget(parent)
{

    QGridLayout *mainLayout = new QGridLayout(this);

    // Title Panel
    QWidget *titlePanel = createHeading("Pollutant Overview Page", TITLE_SIZE);
    titlePanel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    // Compliance Indicator
    initChart();
    // Dynamically update thresholds if needed
    // complianceIndicator->ubdateIndicator(20, 60, 100);

    // Placeholder
    QWidget *placeholder = createFrame();
    QVBoxLayout *placeholderLayout = new QVBoxLayout(placeholder);
    placeholderLayout->addWidget(createHeading("Additional Info", SUBHEADING_SIZE));

    // Back Button
    backButton = new QPushButton("Back to Dashboard", this);
    connect(backButton, &QPushButton::clicked, this, &PollutantOverviewPage::goBack);

    // Add Widgets to Layout
    mainLayout->addWidget(titlePanel, 0, 0, 1, -1);            // Title panel
    mainLayout->addWidget(createNavigationBar(), 1, 0, 1, -1); // Navigation bar
    if(chartHolder)
        mainLayout->addWidget(chartHolder, 2, 0, 1, -1);   // Compliance Indicator
    mainLayout->addWidget(placeholder, 3, 0, 1, -1);           // Placeholder
    mainLayout->addWidget(backButton, 10, 0, 1, -1);           // Back button
}

QWidget* PollutantOverviewPage::getChart() const
{
    return chartHolder;
}

void PollutantOverviewPage::initChart()
{
    if(chartHolder)
        static_cast<QGridLayout*>(layout())->addWidget(chartHolder, 2, 0, 1, -1);   // Compliance Indicator
    else
    {
        // Data setup
        QStringList names = {"Empty", "..."};
        QList<int> counts = {10, 15};
        QList<double> averages = {3.5, 4.2};

        // Create series
        QBarSet *countSet = new QBarSet("Count");
        QBarSet *avgSet = new QBarSet("Average");

        for (int i = 0; i < names.size(); ++i) {
            *countSet << counts[i];
            *avgSet << averages[i];
        }

        QBarSeries *series = new QBarSeries();
        series->append(countSet);
        series->append(avgSet);

        // Create chart
        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Pollutant Overview");
        chart->setAnimationOptions(QChart::SeriesAnimations);

        // Customize X-axis
        QBarCategoryAxis *axisX = new QBarCategoryAxis();
        axisX->append(names);
        chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);

        // Customize Y-axis
        QValueAxis *axisY = new QValueAxis();
        axisY->setRange(0, 20); // Adjust range as needed
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);

        // Customize legend
        chart->legend()->setVisible(true);
        chart->legend()->setAlignment(Qt::AlignBottom);

        // Create the chart view and show it
        chartHolder = new QChartView(chart);
        chartHolder->setRenderHint(QPainter::Antialiasing);
    }
}

void PollutantOverviewPage::updateChart(QStringList materials, QList<int> counts, QList<double> avgs, int maxCount, double maxAvg)
{
    LOGD(QString("Materials.size: %1, counts.size: %2, avgs.size: %3").arg(materials.size()).arg(counts.size()).arg(avgs.size()));
    auto chart = chartHolder->chart();
    // Clear existing chart
    chart->removeAllSeries();
    chart->removeAxis(chart->axes(Qt::Horizontal).first());
    chart->removeAxis(chart->axes(Qt::Vertical).first());

    // Create new series
    QBarSet *countSet = new QBarSet("Count");
    QBarSet *avgSet = new QBarSet("Average");

    for (int i = 0; i < materials.size(); ++i) {
        *countSet << counts[i];
        *avgSet << avgs[i];
    }

    QBarSeries *series = new QBarSeries();
    series->append(countSet);
    series->append(avgSet);

    // Add series to chart
    chart->addSeries(series);
    chart->setTitle("Pollutant Overview");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Update axes
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(materials);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, std::max((double)maxCount,maxAvg)*1.05);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
}
