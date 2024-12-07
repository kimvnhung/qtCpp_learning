#include "fluorinatedCompoundsPage.hpp"
#include "ui_elements.hpp"
#include <QtWidgets>
#include <QBarCategoryAxis>
#include <QBarSeries>
#include <QBarSet>
#include <QChartView>
#include <QValueAxis>

#include "common.hpp"
FluorinatedCompoundsPage::FluorinatedCompoundsPage(QWidget* parent, QStackedWidget* pageStack)
    : QWidget(parent) {

    // Main layout for page
    QGridLayout* mainLayout = new QGridLayout(this);

    // Title panel with heading
    QWidget* titlePanel = createHeading("Fluorinated Compounds", TITLE_SIZE);
    titlePanel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    // Add a navigation bar
    mainLayout->addWidget(createNavigationBar(pageStack, this), 1, 0, 1, -1); // Navigation bar

    // Compliance index panel
    QWidget* complianceIndexPanel = createFrame();
    QVBoxLayout* complianceIndexLayout = new QVBoxLayout(complianceIndexPanel);
    complianceIndexLayout->addWidget(createHeading("Compliance Index", SUBHEADING_SIZE));
    complianceIndexLayout->addWidget(createParagraph("bla bla bla include some info."));
    complianceIndexLayout->setAlignment(Qt::AlignTop);

    initChart();

    // Tooltop label
    QLabel* infoLabel = new QLabel("Hover over me!", this);
    infoLabel->setStyleSheet("background-color: lightgray; padding: 5px;");
    attachTooltip(infoLabel, this, "Information Label", "This label displays some important information.");

    // Back button to dashboard
    backButton = new QPushButton("Back to Dashboard", this);
    connect(backButton, &QPushButton::clicked, this, &FluorinatedCompoundsPage::goBack);

    // Add widgets to grid layout
    mainLayout->addWidget(titlePanel, 0, 0, 1, -1);
    mainLayout->addWidget(complianceIndexPanel, 2, 0, 8, 2);
    mainLayout->addWidget(infoLabel, 2, 2, 1, 1); 
    if(chartHolder)
            mainLayout->addWidget(chartHolder, 5, 2, 5, 5);
        else
            mainLayout->addWidget(createFrame(), 5, 2, 5, 5);    mainLayout->addWidget(backButton, 10, 0, 1, -1);

    setLayout(mainLayout);
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
        // Add random data to the bar series
        *set0 << 10 << 50 << 20 << 40 << 60 << 80 << 30 << 90 << 70 << 20 << 40 << 60;
        QBarSeries *series = new QBarSeries();
        series->setBarWidth(1);
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
