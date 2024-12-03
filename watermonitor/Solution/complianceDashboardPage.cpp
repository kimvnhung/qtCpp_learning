#include "complianceDashboardPage.hpp"
#include "ui_elements.hpp"
#include <QtWidgets>

ComplianceDashboardPage::ComplianceDashboardPage(QWidget *parent)
    : QWidget(parent)
{

    QGridLayout *mainLayout = new QGridLayout(this);
    // Create page title with a fixed height and flexible width.
    QWidget *titlePanel = createHeading("Compliance Dashboard Page", TITLE_SIZE);
    titlePanel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    // Create container to hold contents
    QWidget *placeholder = createFrame();
    QVBoxLayout *placeholderLayout = new QVBoxLayout(placeholder);
    placeholderLayout->addWidget(createHeading("Placeholder", SUBHEADING_SIZE));

    // Back button to return to dashboard
    backButton = new QPushButton("Back to Dashboard", this);
    connect(backButton, &QPushButton::clicked, this, &ComplianceDashboardPage::goBack);

    // Add widgets to layout (widget name, row, column, row span, column span)
    mainLayout->addWidget(titlePanel, 0, 0, 1, -1);
    mainLayout->addWidget(createNavigationBar(), 1, 0, 1, -1);
    mainLayout->addWidget(placeholder);
    mainLayout->addWidget(backButton, 10, 0, 1, -1);
}
