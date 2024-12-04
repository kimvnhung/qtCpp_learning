#include "POPpage.hpp"
#include "ui_elements.hpp"
#include <QtWidgets>

#include <QCategoryAxis>
#include <QChartView>
#include <QLineSeries>

#include "common.hpp"

QStringList months = {"January", "February", "March", "April", "May", "June",
                      "July", "August", "September", "October", "November", "December"};

POPpage::POPpage(QWidget *parent) : QWidget(parent)
{

    QGridLayout *mainLayout = new QGridLayout(this);
    QWidget *titlePanel = createHeading("Persistent Organic Pollutants", TITLE_SIZE);
    titlePanel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    QWidget *complianceIndexPanel = createFrame();
    QVBoxLayout *complianceIndexLayout = new QVBoxLayout(complianceIndexPanel);
    complianceIndexLayout->addWidget(createHeading("Compliance Index", SUBHEADING_SIZE));
    complianceIndexLayout->addWidget(createParagraph("bla bla bla include some info lskadjlk asdflkj asfl ksalf dk"));
    complianceIndexLayout->setAlignment(Qt::AlignTop);

    initChart();

    QWidget *informationPanel = createFrame();
    QVBoxLayout *informationLayout = new QVBoxLayout(informationPanel);
    informationLayout->addWidget(createHeading("Data on PCB's and POP's and their impact on the environment and health", SUBHEADING_SIZE));

    mainLayout->addWidget(titlePanel, 0, 0, 1, -1);
    mainLayout->addWidget(createNavigationBar(), 1, 0, 1, -1);
    mainLayout->addWidget(complianceIndexPanel, 2, 0, 8, 2);
    mainLayout->addWidget(informationPanel, 2, 2, 3, 5);
    if(chartHolder)
        mainLayout->addWidget(chartHolder, 5, 2, 5, 5);

    backButton = new QPushButton("Back to Dashboard", this);
    connect(backButton, &QPushButton::clicked, this, &POPpage::goBack);

    mainLayout->addWidget(backButton, 10, 0, 1, -1);
}

QWidget* POPpage::getChart() const
{
    return chartHolder;
}

void POPpage::initChart()
{
    LOG();
    if(chartHolder)
    {
        LOG();
        static_cast<QGridLayout*>(layout())->addWidget(chartHolder, 5, 2, 5, 5);
    }
    else
    {
        // Initial chart setup
        QChart *chart = new QChart();

        // Create initial data
        lineSeries = new QLineSeries();

        // add sample data
        for (int i = 0; i < months.size(); ++i) {
            lineSeries->append(i, rand() % 100);
        }

        chart->addSeries(lineSeries);
        // X-Axis: Month names
        QCategoryAxis *axisX = new QCategoryAxis();
        for (int i = 0; i < months.size(); ++i) {
            axisX->append(months[i], i);
        }
        axisX->setRange(0,11);

        chart->addAxis(axisX, Qt::AlignBottom);
        lineSeries->attachAxis(axisX);

        // Y-Axis: Values
        QValueAxis *axisY = new QValueAxis();
        axisY->setRange(0, 100); // Assuming values are between 0 and 100
        axisY->setTitleText("Value");
        chart->addAxis(axisY, Qt::AlignLeft);
        lineSeries->attachAxis(axisY);

        chart->setTitle("Persistent Organic Pollutants");
        chart->axes(Qt::Horizontal).first()->setTitleText("Time");
        chart->axes(Qt::Vertical).first()->setTitleText("Notation");

        chartHolder = new QChartView(chart);
        chartHolder->setRenderHint(QPainter::Antialiasing);
    }

}



void POPpage::updateChart(QList<double> values, double max, double min)
{
    // LOGD(QString("Updating chart with %1 values, max=%2, min=%3").arg(values.size()).arg(max).arg(min));

    // Update the corresponding point in the series
    for (int i = 0; i < values.size(); ++i) {
        // LOGD(QString("value[%1]=%2").arg(i).arg(values[i]));
        lineSeries->replace(i, i, values[i]);
    }
    // set range in y axis
    chartHolder->chart()->axes(Qt::Vertical).first()->setRange(min, max);
}
