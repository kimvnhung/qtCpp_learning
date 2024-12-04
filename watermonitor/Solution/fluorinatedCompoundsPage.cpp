#include "fluorinatedCompoundsPage.hpp"
#include "ui_elements.hpp"
#include <QtWidgets>

#include <QBarCategoryAxis>
#include <QBarSeries>
#include <QBarSet>
#include <QChartView>
#include <QValueAxis>

#include "common.hpp"

FluorinatedCompoundsPage::FluorinatedCompoundsPage(QWidget *parent) : QWidget(parent)
{
    QGridLayout *mainLayout = new QGridLayout(this);
    QWidget *titlePanel = createHeading("Fluorinated Compounds", TITLE_SIZE);
    titlePanel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    QWidget *complianceIndexPanel = createFrame();
    QVBoxLayout *complianceIndexLayout = new QVBoxLayout(complianceIndexPanel);
    complianceIndexLayout->addWidget(createHeading("Compliance Index", SUBHEADING_SIZE));
    complianceIndexLayout->addWidget(createParagraph("bla bla bla include some info lskadjlk asdflkj asfl ksalf dk"));
    complianceIndexLayout->setAlignment(Qt::AlignTop);

    initChart();

    QWidget *informationPanel = createFrame();
    QVBoxLayout *informationLayout = new QVBoxLayout(informationPanel);
    informationLayout->addWidget(createHeading("Display Levels of PFA's", SUBHEADING_SIZE));

    mainLayout->addWidget(titlePanel, 0, 0, 1, -1);
    mainLayout->addWidget(createNavigationBar(), 1, 0, 1, -1);
    mainLayout->addWidget(complianceIndexPanel, 2, 0, 8, 2);
    mainLayout->addWidget(informationPanel, 2, 2, 3, 5);
    if(chartHolder)
        mainLayout->addWidget(chartHolder, 5, 2, 5, 5);
    else
        mainLayout->addWidget(createFrame(), 5, 2, 5, 5);

    backButton = new QPushButton("Back to Dashboard", this);
    connect(backButton, &QPushButton::clicked, this, &FluorinatedCompoundsPage::goBack);

    mainLayout->addWidget(backButton, 10, 0, 1, -1);
}

void FluorinatedCompoundsPage::initChart()
{
    LOG();
    if(chartHolder)
    {
        static_cast<QGridLayout*>(layout())->addWidget(chartHolder, 5, 2, 5, 5);
    }
    else
    {
        // Create bar series
        QBarSet *set0 = new QBarSet("PFA's");
        *set0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0;
        QBarSeries *series = new QBarSeries();
        series->append(set0);

        // Create chart and add series
        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("PFA's");
        chart->setAnimationOptions(QChart::SeriesAnimations);

        // Create categories
        QStringList categories;
        categories << "Jan" << "Feb" << "Mar" << "Apr" << "May" << "Jun" << "Jul" << "Aug" << "Sep" << "Oct" << "Nov" << "Dec";
        QBarCategoryAxis *axisX = new QBarCategoryAxis();
        axisX->append(categories);
        chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);

        // Create value axis
        QValueAxis *axisY = new QValueAxis();
        axisY->setRange(0, 100);
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);

        // Create chart view
        chartHolder = new QChartView(chart);
        chartHolder->setRenderHint(QPainter::Antialiasing);
    }
}

void FluorinatedCompoundsPage::updateChart()
{
    // Update the chart with new data
    // This function is called when new data is available or when the user interacts with the UI
}

QWidget* FluorinatedCompoundsPage::getChart() const
{
    return chartHolder;
}
