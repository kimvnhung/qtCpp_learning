#include "geographicalHotspotsPage.hpp"
#include "ui_elements.hpp"
#include <QtWidgets>

GeographicalHotspotsPage::GeographicalHotspotsPage(QWidget *parent)
    : QWidget(parent)
{

    QGridLayout *mainLayout = new QGridLayout(this);
    QWidget *titlePanel = createHeading("Geographical Hotspots Page", TITLE_SIZE);
    titlePanel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    QWidget *placeholder = createFrame();
    QVBoxLayout *placeholderLayout = new QVBoxLayout(placeholder);
    placeholderLayout->addWidget(createHeading("Placeholder", SUBHEADING_SIZE));

    backButton = new QPushButton("Back to Dashboard", this);
    connect(backButton, &QPushButton::clicked, this, &GeographicalHotspotsPage::goBack);

    mainLayout->addWidget(titlePanel, 0, 0, 1, -1);
    mainLayout->addWidget(createNavigationBar(), 1, 0, 1, -1);
    mainLayout->addWidget(placeholder);
    mainLayout->addWidget(backButton, 10, 0, 1, -1);
}
