#include "dashboardpage.hpp"
#include "ui_elements.hpp"
#include <QtWidgets>

#include "common.hpp"

DashboardPage::DashboardPage(QWidget* parent, QStackedWidget* pageStack
    , QWidget *pollutantsOverviewChart
    , QWidget *complianceDashboardChart
    , QWidget *fluorinatedCompoundsChart
    , QWidget *environmentalLitterIndicatorsChart
    , QWidget *popChart
    , QWidget *geographicalHotspotsChart
    , DataHandler* dataHandler)
  : QWidget(parent) {
  // Central panel to store "cards"
  QWidget* contentPanel = createFrame();
  
  // change to dynamic size 
  contentPanel->setFixedWidth(900);
  contentPanel->setFixedHeight(720);

  // Side panel for Search, Time filter and Load CSV button
  QWidget* sidePanel = createSidePanel();
  sidePanel->setFixedWidth(200);

  QWidget *searchBar = createSearchBar();
    timePeriodFilter = new CreateFilters(sidePanel);
  connect(timePeriodFilter, &CreateFilters::filterChanged, [this](const QString &buttonText)
          {
      if(buttonText == "Past 30 Days")
          curFilter = FilterType::PAST_30_DAYS;
      else if(buttonText == "Past 3 Months")
          curFilter = FilterType::PAST_3_MONTHS;
      else if(buttonText == "Past 6 Months")
          curFilter = FilterType::PAST_6_MONTHS;
      else if(buttonText == "Year Till Date")
          curFilter = FilterType::YEAR_TILL_DATE;
      else if(buttonText == "Past 1 Year")
          curFilter = FilterType::PAST_YEAR;
      else if(buttonText == "All Time")
          curFilter = FilterType::ALL_TIME;

      LOGD(QString("Current Filter: %1").arg((int)curFilter));
  });

  // Create dashboard cards and nav buttons
  dashboardCards = createDashboardCards(pageStack, this, dataHandler, pollutantsOverviewChart, complianceDashboardChart, fluorinatedCompoundsChart, environmentalLitterIndicatorsChart, popChart, geographicalHotspotsChart);
  QWidget* navBar = createNavigationBar(pageStack, this);

  // Initialise time period filter and load button
  timePeriodFilter = new CreateFilters(sidePanel);
  timePeriodFilter->addTimePeriodButtons();
  QPushButton* loadButton = new QPushButton("Load CSV");

  // Link button click event with the DashboardPage::loadCSV event slot and loadCSV();
  connect(loadButton, &QPushButton::clicked, this, &DashboardPage::loadCSV);
  
  // Add widgets to the side panel
  QVBoxLayout* sidePanelLayout = new QVBoxLayout(sidePanel);
  sidePanel->setLayout(sidePanelLayout);
  sidePanel->layout()->addWidget(searchBar);
  sidePanel->layout()->addWidget(timePeriodFilter->getFiltersFrame());
  sidePanel->layout()->addWidget(loadButton);

  // Add dashboard cards to content panel
  QVBoxLayout* contentPanelLayout = new QVBoxLayout(contentPanel);
  contentPanel->setLayout(contentPanelLayout);
  contentPanel->layout()->addWidget(dashboardCards);
  
  // Create main layout
  // The GridLayout systems takes in (Widget, row, col, rowspan, colspan) - use for info pages too
  // -1 row/col span means it spans entire row/col - makes for easy layout management.
  QGridLayout* mainLayout = new QGridLayout(this);
  mainLayout->addWidget(createTitleHeader(), 0, 0, 1, -1);
  mainLayout->addWidget(navBar, 1, 0, 1, -1);
  mainLayout->addWidget(sidePanel, 2, 0, 8, 1);
  mainLayout->addWidget(contentPanel, 2, 1, 8, 3);
  
  setLayout(mainLayout);
}


void DashboardPage::updateStatus(const QString &message)
{
  statusLabel->setText(message);
}

void DashboardPage::reloadCharts()
{
    const QObjectList& children = dashboardCards->children();
    for (QObject* child : children) {
        QWidget* widget = qobject_cast<QWidget*>(child);
        if (widget) {
            // Access the layout of the widget (card)
            QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(widget->layout());
            if (layout) {
                for (int i = 0; i < layout->count(); ++i) {
                    QWidget* childWidget = layout->itemAt(i)->widget();
                    if (childWidget) {
                        // Check for graph widgets based on a custom property
                        
                            // Call reloadData() via a dynamic method call
                            QMetaObject::invokeMethod(childWidget, "reloadData", Qt::DirectConnection);
                        
                    }
                }
            }
        }
    }
}




DashboardPage::FilterType DashboardPage::currentFilter() const
{
    return curFilter;
}
