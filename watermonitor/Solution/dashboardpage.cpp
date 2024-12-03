#include "dashboardpage.hpp"
#include "interactive_card.hpp"
#include "ui_elements.hpp"
#include <QtWidgets>

DashboardPage::DashboardPage(QWidget *parent) : QWidget(parent)
{
  // Central panel to store "cards"
  QWidget *contentPanel = createFrame();
  QGridLayout *cardLayout = new QGridLayout();

  // Side panel for Search, Time filter and Load CSV button
  QWidget *sidePanel = createSidePanel();
  QWidget *searchBar = createSearchBar();
  CreateFilters *timePeriodFilter = new CreateFilters(sidePanel);
  timePeriodFilter->addTimePeriodButtons();
  QPushButton *loadButton = new QPushButton("Load CSV");
  statusLabel = new QLabel("");
  statusLabel->setAlignment(Qt::AlignCenter);

  // Link button click event with the DashboardPage::loadCSV event slot and loadCSV();
  connect(loadButton, &QPushButton::clicked, this, &DashboardPage::loadCSV);
  // Add widgets to the side panel
  sidePanel->layout()->addWidget(searchBar);
  sidePanel->layout()->addWidget(timePeriodFilter->getFiltersFrame());
  sidePanel->layout()->addWidget(loadButton);
  sidePanel->layout()->addWidget(statusLabel);

  // Create cards
  // When the cards are clicked, a callback switches the page to the respected page -> [this](){...}.
  // createCard creates a card with a title, parent widget, callback (found in interactive_card.cpp).
  // Cards are positioned by row, column, rowspan and colspan.
  cardLayout->addWidget(createCard("Pollutants Overview", this, [this]()
                                   { goToPollutantOverviewPage(); }),
                        0, 0, 1, -1);
  cardLayout->addWidget(createCard("Persistent Organic Pollutants", this, [this]()
                                   { goToPOPpage(); }),
                        1, 1);
  cardLayout->addWidget(createCard("Compliance Dashboard", this, [this]()
                                   { goToComplianceDashboardPage(); }),
                        1, 0);
  cardLayout->addWidget(createCard("Fluorinated Compounds", this, [this]()
                                   { goToFluorinatedCompoundsPage(); }),
                        2, 1);
  cardLayout->addWidget(createCard("Environmental Litter Indicators", this, [this]()
                                   { goToEnvironmentalLitterIndicatorsPage(); }),
                        2, 0);
  cardLayout->addWidget(createCard("Raw Data", this, [this]()
                                   { goToRawDataPage(); }),
                        3, 1);
  cardLayout->addWidget(createCard("Geographical Hotspots", this, [this]()
                                   { goToGeographicalHotspotsPage(); }),
                        3, 0);

  contentPanel->setLayout(cardLayout);

  // The GridLayout systems takes in (Widget, row, col, rowspan, colspan) - use for info pages too
  // -1 row/col span means it spans entire row/col - makes for easy layout management.
  QGridLayout *mainLayout = new QGridLayout(this);
  mainLayout->addWidget(createTitleHeader(), 0, 0, 1, -1);
  mainLayout->addWidget(createNavigationBar(), 1, 0, 1, -1);
  mainLayout->addWidget(sidePanel, 2, 0, 8, 1);

  // contentPanel is temporary, until navigation bar is and completed -
  // to be replaced with relavent main info content
  mainLayout->addWidget(contentPanel, 2, 1, 8, 3);
}

void DashboardPage::updateStatus(const QString &message)
{
  statusLabel->setText(message);
}

// Implementing card interaction.
QWidget *DashboardPage::createCard(const QString &title, QWidget *parent, std::function<void()> onClickCallback)
{
  // Create new card object with title and parent of the card widget.
  interactiveCard *card = new interactiveCard(title, this);

  // When a card is clicked, interactiveCard emits cardClicked signal.
  // [this, title]() {...} determines which function to call based on the title.
  connect(card, &interactiveCard::cardClicked, this, [this, title]()
          {
    if (title == "Persistent Organic Pollutants") {
      goToPOPpage();
    } else if (title == "Pollutants Overview") {
      goToPollutantOverviewPage();
    } else if (title == "Compliance Dashboard") {
      goToComplianceDashboardPage();
    } else if (title == "Fluorinated Compounds") {
      goToFluorinatedCompoundsPage();
    } else if (title == "Environmental Litter Indicators") {
      goToEnvironmentalLitterIndicatorsPage();
    } else if (title == "Raw Data") {
      goToRawDataPage();
    } else if (title == "Geographical Hotspots") {
      goToGeographicalHotspotsPage();
    } });

  return card;
}
