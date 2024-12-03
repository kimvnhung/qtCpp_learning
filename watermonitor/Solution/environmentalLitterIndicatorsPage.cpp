#include "environmentalLitterIndicatorsPage.hpp"
#include <QtWidgets>
#include "ui_elements.hpp"

EnvironmentalLitterIndicatorsPage::EnvironmentalLitterIndicatorsPage(QWidget *parent)
    : QWidget(parent)
{

    QGridLayout *mainLayout = new QGridLayout(this);
    QWidget *titlePanel = createHeading("Environmental Litter Indicators", TITLE_SIZE);
    titlePanel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    QWidget *complianceIndexPanel = createFrame();
    QVBoxLayout *complianceIndexLayout = new QVBoxLayout(complianceIndexPanel);
    complianceIndexLayout->addWidget(createHeading("Compliance Index", SUBHEADING_SIZE));
    complianceIndexLayout->addWidget(createParagraph("bla bla bla include some info lskadjlk asdflkj asfl ksalf dk"));
    complianceIndexLayout->setAlignment(Qt::AlignTop);

    QWidget *informationPanel = createFrame();
    QVBoxLayout *informationLayout = new QVBoxLayout(informationPanel);
    informationLayout->addWidget(createHeading("Summary of physical pollutants", SUBHEADING_SIZE));

    QWidget *filterPanel = createFrame();
    QHBoxLayout *filterLayout = new QHBoxLayout(filterPanel);
    CreateFilters *customFilters = new CreateFilters(filterPanel);
    customFilters->changeToHorizontalLayout();
    customFilters->addCustomButton("filter 1");
    customFilters->addCustomButton("filter 2");
    customFilters->addCustomButton("filter 3");
    filterLayout->addWidget(customFilters->getFiltersFrame());

    QWidget *graphPanel = createFrame();
    QVBoxLayout *graphLayout = new QVBoxLayout(graphPanel);
    graphLayout->addWidget(createHeading("Comparisons across different locations using bar/pie chart", SUBHEADING_SIZE));

    mainLayout->addWidget(titlePanel, 0, 0, 1, -1);
    mainLayout->addWidget(createNavigationBar(), 1, 0, 1, -1);
    mainLayout->addWidget(complianceIndexPanel, 2, 0, 8, 2);
    mainLayout->addWidget(informationPanel, 2, 2, 2, 5);
    mainLayout->addWidget(filterPanel, 4, 2, 1, 5);
    mainLayout->addWidget(graphPanel, 5, 2, 5, 5);

    backButton = new QPushButton("Back to Dashboard", this);
    connect(backButton, &QPushButton::clicked, this, &EnvironmentalLitterIndicatorsPage::goBack);

    mainLayout->addWidget(backButton, 10, 0, 1, -1);
}
