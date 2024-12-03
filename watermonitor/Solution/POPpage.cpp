#include "POPpage.hpp"
#include "ui_elements.hpp"
#include <QtWidgets>

POPpage::POPpage(QWidget* parent): QWidget(parent) 
{

    QGridLayout* mainLayout = new QGridLayout(this);
    QWidget* titlePanel = createHeading("Persistent Organic Pollutants", TITLE_SIZE);
    titlePanel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    QWidget* complianceIndexPanel = createFrame();
    QVBoxLayout* complianceIndexLayout = new QVBoxLayout(complianceIndexPanel);
    complianceIndexLayout->addWidget(createHeading("Compliance Index", SUBHEADING_SIZE));
    complianceIndexLayout->addWidget(createParagraph("bla bla bla include some info lskadjlk asdflkj asfl ksalf dk"));
    complianceIndexLayout->setAlignment(Qt::AlignTop);

    QWidget* graphPanel = createFrame();
    QVBoxLayout* graphLayout = new QVBoxLayout(graphPanel);
    graphLayout->addWidget(createHeading("Graph", SUBHEADING_SIZE));
    
    QWidget* informationPanel = createFrame();
    QVBoxLayout* informationLayout = new QVBoxLayout(informationPanel);
    informationLayout->addWidget(createHeading("Data on PCB's and POP's and their impact on the environment and health", SUBHEADING_SIZE));

    mainLayout->addWidget(titlePanel, 0, 0, 1, -1);
    mainLayout->addWidget(createNavigationBar(), 1, 0, 1, -1);
    mainLayout->addWidget(complianceIndexPanel, 2, 0, 8, 2);
    mainLayout->addWidget(informationPanel, 2, 2, 3, 5);
    mainLayout->addWidget(graphPanel, 5, 2, 5, 5);

    backButton = new QPushButton("Back to Dashboard", this);
    connect(backButton, &QPushButton::clicked, this, &POPpage::goBack);

    mainLayout->addWidget(backButton, 10, 0, 1, -1);
}
