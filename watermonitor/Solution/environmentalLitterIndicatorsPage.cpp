#include "environmentalLitterIndicatorsPage.hpp"
#include <QBarCategoryAxis>
#include <QBarSeries>
#include <QBarSet>
#include <QChartView>
#include <QValueAxis>
#include <QtWidgets>
#include "ui_elements.hpp"

#include "common.hpp"

EnvironmentalLitterIndicatorsPage::EnvironmentalLitterIndicatorsPage(QWidget *parent)
    : QWidget(parent)
{

    QGridLayout *mainLayout = new QGridLayout(this);
    QWidget *titlePanel = createHeading("Environmental Litter Indicators", TITLE_SIZE);
    titlePanel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    QWidget *complianceIndexPanel = createFrame();
    QVBoxLayout *complianceIndexLayout = new QVBoxLayout(complianceIndexPanel);
    complianceIndexLayout->addWidget(createHeading("Compliance Index", SUBHEADING_SIZE));
    complianceIndexLayout->addWidget(createParagraph("bla bla bla include some info lskadjlk asdflkj asfl ksalf dk"));
    complianceIndexLayout->setAlignment(Qt::AlignTop);

    QWidget *informationPanel = createFrame();
    QVBoxLayout *informationLayout = new QVBoxLayout(informationPanel);
    informationLayout->addWidget(createHeading("Summary of physical pollutants", SUBHEADING_SIZE));

    QWidget *filterPanel = createFrame();
    QHBoxLayout *filterLayout = new QHBoxLayout(filterPanel);
    CreateFilters *customFilters = new CreateFilters(filterPanel);
    customFilters->changeToHorizontalLayout();
    customFilters->addCustomButton("filter 1");
    customFilters->addCustomButton("filter 2");
    customFilters->addCustomButton("filter 3");
    filterLayout->addWidget(customFilters->getFiltersFrame());

    initChart();

    mainLayout->addWidget(titlePanel, 0, 0, 1, -1);
    mainLayout->addWidget(createNavigationBar(), 1, 0, 1, -1);
    mainLayout->addWidget(complianceIndexPanel, 2, 0, 8, 2);
    mainLayout->addWidget(informationPanel, 2, 2, 2, 5);
    mainLayout->addWidget(filterPanel, 4, 2, 1, 5);
    if(chartHolder)
        mainLayout->addWidget(chartHolder, 5, 2, 5, 5);
    else
        mainLayout->addWidget(createFrame(), 5, 2, 5, 5);

    backButton = new QPushButton("Back to Dashboard", this);
    connect(backButton, &QPushButton::clicked, this, &EnvironmentalLitterIndicatorsPage::goBack);

    mainLayout->addWidget(backButton, 10, 0, 1, -1);
}

void EnvironmentalLitterIndicatorsPage::initChart()
{
    LOG();
    if(chartHolder)
    {
        static_cast<QGridLayout*>(layout())->addWidget(chartHolder, 5, 2, 5, 5);
    }
    else
    {
        // Create bar sets for each category (e.g., "Product A", "Product B")
        QBarSet *set0 = new QBarSet("Product A");
        *set0 << 10 << 20 << 30 << 40 << 50 << 60;

        QBarSet *set1 = new QBarSet("Product B");
        *set1 << 20 << 30 << 40 << 50 << 60 << 70;

        QBarSet *set2 = new QBarSet("Product C");
        *set2 << 30 << 40 << 50 << 60 << 70 << 80;

        // Create a Bar Series and add the sets to it
        QBarSeries *series = new QBarSeries();
        series->append(set0);
        series->append(set1);
        series->append(set2);

        // Create a chart to hold the series
        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Environmental Litter Indicators");

        // Set categories for the X-axis (e.g., months)
        QStringList categories;
        categories << "Jan" << "Feb" << "Mar" << "Apr" << "May" << "Jun";

        QBarCategoryAxis *axisX = new QBarCategoryAxis();
        axisX->append(categories);
        chart->addAxis(axisX, Qt::AlignBottom);

        // Create a value axis for the Y-axis (e.g., sales figures)
        QValueAxis *axisY = new QValueAxis();
        axisY->setRange(0, 300);  // Adjust range based on your data
        chart->addAxis(axisY, Qt::AlignLeft);

        // Attach the axis to the series
        series->attachAxis(axisX);
        series->attachAxis(axisY);

        // Create a chart view to display the chart
        chartHolder = new QChartView(chart);
        chartHolder->setRenderHint(QPainter::Antialiasing);
    }
}

void EnvironmentalLitterIndicatorsPage::updateChart()
{

}

QWidget* EnvironmentalLitterIndicatorsPage::getChart() const
{
    return chartHolder;
}
