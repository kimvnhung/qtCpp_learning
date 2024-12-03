#include "pollutantOverviewPage.hpp"
#include "ui_elements.hpp"
#include <QtWidgets>

PollutantOverviewPage::PollutantOverviewPage(QWidget* parent)
    : QWidget(parent) {

    QGridLayout* mainLayout = new QGridLayout(this);

    // Title Panel
    QWidget* titlePanel = createHeading("Pollutant Overview Page", TITLE_SIZE);
    titlePanel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    // Compliance Indicator
    QWidget* complianceIndicator = createComplianceIndicator(10, 60, 70, 55,this);
    // Dynamically update thresholds if needed
    //complianceIndicator->ubdateIndicator(20, 60, 100);

    // Placeholder
    QWidget* placeholder = createFrame();
    QVBoxLayout* placeholderLayout = new QVBoxLayout(placeholder);
    placeholderLayout->addWidget(createHeading("Additional Info", SUBHEADING_SIZE));

    // Back Button
    backButton = new QPushButton("Back to Dashboard", this);
    connect(backButton, &QPushButton::clicked, this, &PollutantOverviewPage::goBack);

    // Add Widgets to Layout
    mainLayout->addWidget(titlePanel, 0, 0, 1, -1);       // Title panel
    mainLayout->addWidget(createNavigationBar(), 1, 0, 1, -1); // Navigation bar
    mainLayout->addWidget(complianceIndicator, 2, 0, 1, -1); // Compliance Indicator
    mainLayout->addWidget(placeholder, 3, 0, 1, -1);       // Placeholder
    mainLayout->addWidget(backButton, 10, 0, 1, -1);      // Back button
}
