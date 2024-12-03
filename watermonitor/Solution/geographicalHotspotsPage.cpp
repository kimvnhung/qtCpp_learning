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
        connect(dataHandler, &DataHandler::geographicalDataReady, this, &GeographicalHotspotsPage::updateHeatMap);
        connect(dataHandler, &DataHandler::handlingGeographicalData,this, &GeographicalHotspotsPage::onHandling);
    }
    else
        LOGD("DataHandler is null");

    QGridLayout *mainLayout = new QGridLayout(this);
    QWidget *titlePanel = createHeading("Geographical Hotspots Page", TITLE_SIZE);
    titlePanel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    placeholder = new QWidget(this);

    backButton = new QPushButton("Back to Dashboard", this);
    connect(backButton, &QPushButton::clicked, this, &GeographicalHotspotsPage::goBack);

    mainLayout->addWidget(titlePanel, 0, 0, 1, -1);
    mainLayout->addWidget(createNavigationBar(), 1, 0, 1, -1);
    mainLayout->addWidget(placeholder);
    mainLayout->addWidget(backButton, 10, 0, 1, -1);
    setLayout(mainLayout);
    initHeatMap();
}

void GeographicalHotspotsPage::initHeatMap()
{
    LOG();
    QStringList months = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    QStringList people = {};
    // Create the heat map
    heatMap = new QTableWidget(this);
    heatMap->setRowCount(people.size());
    heatMap->setColumnCount(months.size());

    // Set the row and column headers (for months and people)
    heatMap->setHorizontalHeaderLabels(months);
    // heatMap->setVerticalHeaderLabels(people);

    // Set the cell values and color each cell based on the frequency
    // for (int row = 0; row < people.size(); ++row) {
    //     for (int col = 0; col < months.size(); ++col) {
    //         int value = rand() % 15; // Random value for demonstration

    //         // Create a table item and set the frequency value as text
    //         QTableWidgetItem* item = new QTableWidgetItem(QString::number(value));

    //         heatMap->setItem(row, col, item);

    //         // Determine the color based on the value (heat map colorization)
    //         QColor color;
    //         if (value <= 5) {
    //             color = QColor(255, 0, 0); // Red for values 0-5
    //         } else if (value <= 10) {
    //             color = QColor(0, 255, 0); // Green for values 5-10
    //         } else {
    //             color = QColor(0, 0, 255); // Blue for values > 10
    //         }

    //         // Apply the color to the cell
    //         item->setBackground(QBrush(color));
    //     }
    // }

    // Resize the columns and rows to fit the content
    // heatMap->resizeColumnsToContents();
    // heatMap->resizeRowsToContents();
    QVBoxLayout *grid = new QVBoxLayout(placeholder);
    grid->addWidget(heatMap);
    placeholder->setLayout(grid);

    // Set size policy to expand and fit
    heatMap->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Remove scrollbars
    // heatMap->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    heatMap->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Make rows and columns stretch to fill the available space
    heatMap->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    heatMap->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // Remove row and column spacing
    heatMap->setShowGrid(false); // Remove grid lines
    // Set the table background to white
    heatMap->setStyleSheet(R"(
    QTableWidget::item
    {
        border: 0px solid;
        background: transparent;
        border-radius: 0px
        margin: 0px;
        padding: 0px;
    }
)");

    // set for cant edit
    heatMap->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // set for unclicable
    heatMap->setSelectionMode(QAbstractItemView::NoSelection);


}

void GeographicalHotspotsPage::resizeEvent(QResizeEvent *event)
{
    LOG();
    // Call the base class implementation to ensure normal resizing behavior
    QWidget::resizeEvent(event);


    LOGD(QString("newSize: %1x%2").arg(placeholder->size().width()).arg(placeholder->size().height()));

    // set all column width equal to placeholder width/13
    QSize newSize = placeholder->size();
    for (int i = 0; i < 13; i++) {
        heatMap->setColumnWidth(i, newSize.width() / 13);
    }
}

void GeographicalHotspotsPage::onHandling(int percent) {
    LOG();
    emit handling(percent, "Geographical Hotspots", "Processing data...");
}

void GeographicalHotspotsPage::updateHeatMap(QStringList locations, QMap<QString,QList<int>> frequency, int min, int max) {
    LOG();

    //set to heatMap
    heatMap->setRowCount(locations.size());
    heatMap->setVerticalHeaderLabels(locations);

    int count = 0;
    int size = heatMap->rowCount() * heatMap->columnCount();

    // Set the cell values and color each cell based on the frequency
    for (int row = 0; row < locations.size(); ++row) {
        for (int col = 0; col < heatMap->columnCount(); ++col) {
            LOGD(QString("row: %1, col: %2").arg(row).arg(col));
            // get header text
            QString month = heatMap->horizontalHeaderItem(col)->text();
            int value = frequency[month][row]; // Random value for demonstration
            LOGD(QString("row: %1, col: %2").arg(row).arg(col));
            // Create a table item and set the frequency value as text
            QTableWidgetItem* item = new QTableWidgetItem(QString::number(value));
            // set for alignment center of the text
            item->setTextAlignment(Qt::AlignCenter);

            heatMap->setItem(row, col, item);
            LOGD(QString("row: %1, col: %2").arg(row).arg(col));
            // Determine the color based on the value (heat map colorization)
            QColor color;
            /*
             * Value change from white -> green -> yellow -> red, from the lowest value to the highest value
            */
            int r = 0;
            int g = 0;
            int b = 0;
            if (value <= (max - min) / 3) {
                // 0 is white, (max-min)/3 is green
                r = 255 * (1 - value * 1.0f / ((max - min) / 3));
                g = 255;
                b = 255;
            } else if (value <= 2 * (max - min) / 3) {
                // (max-min)/3 is green, 2*(max-min)/3 is yellow
                r = 255 * (value * 1.0f / ((max - min) / 3) - 1);
                g = 255;
            } else {
                // 2*(max-min)/3 is yellow, max-min is red
                r = 255;
                g = 255 * (value * 1.0f / ((max - min) / 3) - 2);
            }

            color = QColor(r, g, b);

            // Apply the color to the cell
            item->setBackground(QBrush(color));
            LOGD(QString("row: %1, col: %2").arg(row).arg(col));
        }
    }

    emit handling(HIDE_PROGRESS_VALUE, "Geographical Hotspots", "Processing data...");

    //Resize the columns and rows to fit the content
    heatMap->resizeColumnsToContents();
    heatMap->resizeRowsToContents();
}


