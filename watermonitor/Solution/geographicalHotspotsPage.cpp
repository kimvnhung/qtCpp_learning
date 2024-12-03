#include "geographicalHotspotsPage.hpp"
#include "ui_elements.hpp"
#include <QBarCategoryAxis>
#include <QBarSeries>
#include <QBarSet>
#include <QCategoryAxis>
#include <QScatterSeries>
#include <QValueAxis>
#include <QtWidgets>

#include "common.hpp"

GeographicalHotspotsPage::GeographicalHotspotsPage(QWidget *parent, DataHandler *dataHandler)
    : QWidget(parent)
    , dataHandler(dataHandler)
{
    if(dataHandler){
        connect(dataHandler, &DataHandler::dataReady, this, &GeographicalHotspotsPage::initChart);
    }
    else
        LOGD("DataHandler is null");

    QGridLayout *mainLayout = new QGridLayout(this);
    QWidget *titlePanel = createHeading("Geographical Hotspots Page", TITLE_SIZE);
    titlePanel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    placeholder = createFrame();
    placeholder->setLayout(new QGridLayout);
    initChart();


    backButton = new QPushButton("Back to Dashboard", this);
    connect(backButton, &QPushButton::clicked, this, &GeographicalHotspotsPage::goBack);

    mainLayout->addWidget(titlePanel, 0, 0, 1, -1);
    mainLayout->addWidget(createNavigationBar(), 1, 0, 1, -1);
    mainLayout->addWidget(placeholder);
    mainLayout->addWidget(backButton, 10, 0, 1, -1);
}

void GeographicalHotspotsPage::initChart()
{
    // Sample data: frequency of person in a month
    // Format: {month, person1, person2, ..., personN}
    QVector<QVector<int>> data = {
        {10, 2, 3, 5}, // January: person1(10), person2(2), person3(3), person4(5)
        {12, 4, 1, 7}, // February: person1(12), person2(4), person3(1), person4(7)
        {8, 1, 2, 3},  // March
        {14, 3, 6, 8}, // April
        {10, 2, 4, 5}  // May
    };

    // List of months
    QStringList months = {"Jan", "Feb", "Mar", "Apr", "May"};

    // List of people
    QStringList people = {"Person 1", "Person 2", "Person 3", "Person 4"};

    // Create the chart
    QChart* chart = new QChart();
    chart->setTitle("Frequency of People in Each Month");

    // Create a series for each person
    QVector<QBarSet*> barSets;
    for (int i = 0; i < people.size(); ++i) {
        QBarSet* barSet = new QBarSet(people[i]);
        for (int j = 0; j < data.size(); ++j) {
            barSet->append(data[j][i]); // Frequency for each person in each month
        }
        barSets.append(barSet);
    }

    // Create a bar series and add the bar sets
    QBarSeries* series = new QBarSeries();
    for (auto barSet : barSets) {
        series->append(barSet);
    }

    // Add the series to the chart
    chart->addSeries(series);

    // Create and configure the X-axis (months)
    QBarCategoryAxis* axisX = new QBarCategoryAxis();
    axisX->append(months); // Set the months as categories for X-axis
    chart->setAxisX(axisX, series);

    // Create and configure the Y-axis (people)
    QValueAxis* axisY = new QValueAxis();
    axisY->setRange(0, 20); // Set range for Y-axis, adjust as needed
    chart->setAxisY(axisY, series);

    // Create the chart view
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    placeholder->layout()->addWidget(chartView);
}

// Function to map a value to a color
QColor GeographicalHotspotsPage::valueToColor(double value, double min, double max) {
    // Normalize the value
    double normalized = (value - min) / (max - min);
    normalized = std::clamp(normalized, 0.0, 1.0);

    // Interpolate between blue (low) and red (high)
    int r = static_cast<int>(normalized * 255);
    int g = 0;
    int b = static_cast<int>((1.0 - normalized) * 255);
    return QColor(r, g, b);
}

// Function to create the heat map series
QScatterSeries* GeographicalHotspotsPage::createHeatMapSeries(const std::vector<std::vector<double>>& data, double min, double max) {
    QScatterSeries* series = new QScatterSeries();
    series->setMarkerShape(QScatterSeries::MarkerShapeRectangle);
    series->setMarkerSize(10.0); // Adjust marker size for better visibility

    int rows = data.size();
    int cols = data[0].size();

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            double value = data[y][x];
            QColor color = valueToColor(value, min, max);
            series->append(x, y); // Add data point
            series->setColor(color); // Set the color for the marker
        }
    }
    return series;
}
