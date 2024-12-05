#include "complianceDashboardPage.hpp"
#include "ui_elements.hpp"
#include <QtWidgets>

#include <QChartView>
#include <QPieSeries>

#include "common.hpp"

ComplianceDashboardPage::ComplianceDashboardPage(QWidget *parent)
    : QWidget(parent)
{

    QGridLayout *mainLayout = new QGridLayout(this);
    // Create page title with a fixed height and flexible width.
    QWidget *titlePanel = createHeading("Compliance Dashboard Page", TITLE_SIZE);
    titlePanel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    // Create container to hold contents
    initChart();

    // Back button to return to dashboard
    backButton = new QPushButton("Back to Dashboard", this);
    connect(backButton, &QPushButton::clicked, this, &ComplianceDashboardPage::goBack);

    // Add widgets to layout (widget name, row, column, row span, column span)
    mainLayout->addWidget(titlePanel, 0, 0, 1, -1);
    mainLayout->addWidget(createNavigationBar(), 1, 0, 1, -1);

    if(chartHolder)
        mainLayout->addWidget(chartHolder,2,0,1,-1);
    else
        mainLayout->addWidget(createFrame(), 2, 0, 1,-1);
    mainLayout->addWidget(backButton, 10, 0, 1, -1);
}

QWidget* ComplianceDashboardPage::getChart() const
{
    return chartHolder;
}

void ComplianceDashboardPage::initChart()
{
    LOG();

    if(chartHolder)
    {
        static_cast<QGridLayout*>(layout())->addWidget(chartHolder,2,0,1,-1);
        acceptClickEvents(chartHolder);
    }
    else {
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

        // Display chart in a view
        chartHolder = new QChartView(chart);
        // Disable click and hover events
        chartHolder->setRenderHint(QPainter::Antialiasing);
        chartHolder->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    }
}

void ComplianceDashboardPage::updateChart(int trueCount, int falseCount)
{
    LOG();
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
