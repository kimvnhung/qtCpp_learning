#include "dashboard.h"
#include <QApplication>
#include <QDebug>

Dashboard::Dashboard(QWidget *parent)
    : QWidget(parent)
{
    mainLayout = new QVBoxLayout(this);

    // Setup Header
    setupHeader();

    // Setup Main Content
    setupMainContent();

    // Setup Footer
    setupFooter();

    setLayout(mainLayout);

    // Language selector
    languageSelector->addItem(tr("English"), "en");
    languageSelector->addItem(tr("Spanish"), "es");
    languageSelector->addItem(tr("French"), "fr");
    languageSelector->addItem(tr("German"), "de");

    // Connect language change logic
    connect(languageSelector, &QComboBox::currentIndexChanged, this, [this](int index) {
        QString languageCode = languageSelector->itemData(index).toString();
        if (!loadLanguage(languageCode)) {
            qWarning() << "Switching to default language due to load failure.";
            loadLanguage("en");
        }
    });
}

bool Dashboard::loadLanguage(const QString &languageCode)
{
    qApp->removeTranslator(&translator); // Remove any existing translator

    QString filePath = QCoreApplication::applicationDirPath() + "/translations/" + QString("water_%1.qm").arg(languageCode);

    if (!translator.load(filePath)) {
        qWarning() << "Failed to load translation file:" << filePath;
        return false;
    }

    qApp->installTranslator(&translator);
    retranslateUi(); // Update the UI with new translations
    qDebug() << "Switched to language code:" << languageCode;
    return true;
}




void Dashboard::setupHeader()
{
    appTitleLabel = new QLabel(tr("Water Quality Monitor"));
    appTitleLabel->setStyleSheet("font-size: 24px; font-weight: bold; margin-bottom: 10px;");

    languageSelector = new QComboBox();
    languageSelector->setToolTip(tr("Select Language"));

    navButtonPollutantOverview = new QPushButton(tr("Pollutant Overview"));
    navButtonPOPs = new QPushButton(tr("POPs"));
    navButtonEnvLitterIndicators = new QPushButton(tr("Environmental Litter Indicators"));
    navButtonFluorinatedCompounds = new QPushButton(tr("Fluorinated Compounds"));
    navButtonCompliance = new QPushButton(tr("Compliance"));

    QHBoxLayout *headerLayout = new QHBoxLayout();
    headerLayout->addWidget(appTitleLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(languageSelector);

    QHBoxLayout *navLayout = new QHBoxLayout();
    navLayout->addWidget(navButtonPollutantOverview);
    navLayout->addWidget(navButtonPOPs);
    navLayout->addWidget(navButtonEnvLitterIndicators);
    navLayout->addWidget(navButtonFluorinatedCompounds);
    navLayout->addWidget(navButtonCompliance);

    mainLayout->addLayout(headerLayout);
    mainLayout->addLayout(navLayout);
}

void Dashboard::setupMainContent()
{
    QGridLayout *gridLayout = new QGridLayout();

    gridLayout->addWidget(createPollutantCard(tr("Pollutant Overview"), tr("Average Levels: 42 µg/L"), "green"), 0, 0);
    gridLayout->addWidget(createPollutantCard(tr("POPs"), tr("Average Levels: 15 µg/L"), "yellow"), 0, 1);
    gridLayout->addWidget(createPollutantCard(tr("Environmental Litter Indicators"), tr("Average Levels: 10 items/m²"), "red"), 1, 0);
    gridLayout->addWidget(createPollutantCard(tr("Fluorinated Compounds"), tr("Average Levels: 5 µg/L"), "green"), 1, 1);
    gridLayout->addWidget(createPollutantCard(tr("Compliance"), tr("90% Compliance"), "green"), 2, 0, 1, 2);

    mainLayout->addLayout(gridLayout);
}

void Dashboard::setupFooter()
{
    QHBoxLayout *footerLayout = new QHBoxLayout();

    QLabel *filterLabel = new QLabel(tr("Filter by:"));
    QComboBox *timeFilter = new QComboBox();
    timeFilter->addItems({tr("Last Month"), tr("Last Year")});
    timeFilter->setObjectName("timeFilter");

    QComboBox *regionFilter = new QComboBox();
    regionFilter->addItems({tr("Region 1"), tr("Region 2"), tr("Region 3")});
    regionFilter->setObjectName("regionFilter");

    footerLayout->addWidget(filterLabel);
    footerLayout->addWidget(timeFilter);
    footerLayout->addWidget(regionFilter);
    footerLayout->addStretch();

    mainLayout->addLayout(footerLayout);
}

QWidget *Dashboard::createPollutantCard(const QString &title, const QString &summary, const QString &color)
{
    QLabel *titleLabel = new QLabel(title);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 16px;");

    QLabel *summaryLabel = new QLabel(summary);

    QVBoxLayout *cardLayout = new QVBoxLayout();
    cardLayout->addWidget(titleLabel);
    cardLayout->addWidget(summaryLabel);
    cardLayout->addStretch();

    QFrame *card = new QFrame();
    card->setLayout(cardLayout);
    card->setFrameShape(QFrame::StyledPanel);
    card->setStyleSheet(QString("border: 1px solid %1; padding: 10px;").arg(color));

    return card;
}

void Dashboard::retranslateUi()
{
    appTitleLabel->setText(tr("Water Quality Monitor"));
    navButtonPollutantOverview->setText(tr("Pollutant Overview"));
    navButtonPOPs->setText(tr("POPs"));
    navButtonEnvLitterIndicators->setText(tr("Environmental Litter Indicators"));
    navButtonFluorinatedCompounds->setText(tr("Fluorinated Compounds"));
    navButtonCompliance->setText(tr("Compliance"));

    languageSelector->setItemText(0, tr("English"));
    languageSelector->setItemText(1, tr("Spanish"));
    languageSelector->setItemText(2, tr("French"));
    languageSelector->setItemText(3, tr("German"));
    languageSelector->setToolTip(tr("Select Language"));

    QComboBox *timeFilter = findChild<QComboBox *>("timeFilter");
    if (timeFilter) {
        timeFilter->setItemText(0, tr("Last Month"));
        timeFilter->setItemText(1, tr("Last Year"));
    }

    QComboBox *regionFilter = findChild<QComboBox *>("regionFilter");
    if (regionFilter) {
        regionFilter->setItemText(0, tr("Region 1"));
        regionFilter->setItemText(1, tr("Region 2"));
        regionFilter->setItemText(2, tr("Region 3"));
    }
}
