#include "rawDataPage.hpp"
#include "ui_elements.hpp"
#include <QtWidgets>
#include <vector>

RawDataPage::RawDataPage(QWidget *parent)
    : QWidget(parent)
{

    mainLayout = new QGridLayout(this);
    QWidget *titlePanel = createHeading("Raw Data", TITLE_SIZE);
    titlePanel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    QWidget *sidePanel = createSidePanel();
    QWidget *searchBar = createSearchBar();

    CreateFilters *timePeriodFilter = new CreateFilters(sidePanel);
    timePeriodFilter->addTimePeriodButtons();

    QPushButton *loadButton = new QPushButton("Load CSV");
    connect(loadButton, &QPushButton::clicked, this, &RawDataPage::loadCSV);

    backButton = new QPushButton("Back to Dashboard", this);
    connect(backButton, &QPushButton::clicked, this, &RawDataPage::goBack);

    sidePanel->layout()->addWidget(searchBar);
    sidePanel->layout()->addWidget(timePeriodFilter->getFiltersFrame());
    sidePanel->layout()->addWidget(loadButton);

    mainLayout->addWidget(titlePanel, 0, 0, 1, -1);
    mainLayout->addWidget(createNavigationBar(), 1, 0, 1, -1);
    mainLayout->addWidget(sidePanel, 2, 0, 8, 1);
    mainLayout->addWidget(backButton, 10, 0, 1, -1);
}