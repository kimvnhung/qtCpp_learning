#include "generaldashboard.h"
#include "common.h"
#include <QDialog>
#include <QLabel>
#include "handlers/filter.hpp"
GeneralDashboard::GeneralDashboard(QWidget *parent)
    : QWidget{parent}, m_dataHandler{new DataHandler}
{
    initializeUi();
    setUpDataHandler();
}
GeneralDashboard::~GeneralDashboard()
{
    if (m_dataHandler)
    {
        m_dataHandler->stop();
        m_dataHandler->wait();
        delete m_dataHandler;
    }
}
// private
void GeneralDashboard::initializeUi()
{
    m_mainLayout = new QVBoxLayout;
    // Set up the header
    setUpHeader();
    // Set up  the content
    m_contentLayout = new QHBoxLayout;
    setUpSettingPanel();
    setUpChartPanel();
    m_mainLayout->addLayout(m_contentLayout);
    // Set up the status bar
    setUpStatusBar();
    setUpPreventDialog();
    setLayout(m_mainLayout);
    setMinimumSize(1280, 720);
}
void GeneralDashboard::setUpDataHandler()
{
    connect(m_settingPanel, &SettingPanel::csvFileAvailable, m_dataHandler, &DataHandler::loadData);
    connect(m_dataHandler, &DataHandler::processingMessage, this, &GeneralDashboard::setProcessingMessage);
    connect(m_dataHandler, &DataHandler::handling, this, &GeneralDashboard::onProcessing);

    connect(m_dataHandler, &DataHandler::pollutantOverviewDataReady, m_chartPanel, &OverviewChartWidget::onPollutantOverviewChartUpdated);
    connect(m_dataHandler, &DataHandler::environmentalLitterIndicatorsDataReady, m_chartPanel, &OverviewChartWidget::onEnvironmentalLitterIndicatorsChartUpdated);
    connect(m_dataHandler, &DataHandler::flourinatedCompoundsChartDataReady, m_chartPanel, &OverviewChartWidget::onFlourinatedCompoundsChartUpdated);
    connect(m_dataHandler, &DataHandler::popsDataReady, m_chartPanel, &OverviewChartWidget::onPOPsChartUpdated);
    connect(m_dataHandler, &DataHandler::complianceChartDataReady, m_chartPanel, &OverviewChartWidget::onComplianceChartUpdated);
    connect(m_dataHandler, &DataHandler::rawDataReady, m_chartPanel, &OverviewChartWidget::onRawDataUpdated);


    connect(m_dataHandler, &DataHandler::locationsChanged, m_settingPanel, &SettingPanel::setLocationsFilter);
    connect(m_dataHandler, &DataHandler::materialsChanged, m_settingPanel, &SettingPanel::setMaterialFilter);
    connect(m_settingPanel, &SettingPanel::materialFilterChanged, [this](const QStringList &materials)
            { m_dataHandler->addFilter(Filter{Filter::FilterType::MATERIALS_SET, QVariant(materials)}); });
    connect(m_settingPanel, &SettingPanel::locationFilterChanged, [this](const QStringList &locations)
            { m_dataHandler->addFilter(Filter{Filter::FilterType::LOCATIONS_SET, QVariant(locations)}); });
    connect(m_settingPanel, &SettingPanel::timeFilterChanged, [this](int index)
            { m_dataHandler->addFilter(Filter{Filter::FilterType::TIME, QVariant(index)}); });
    connect(m_settingPanel, &SettingPanel::maxLocationsChanged, [this](int maxLocations)
            { m_dataHandler->addFilter(Filter{Filter::FilterType::LOCATIONS_MAX, QVariant(maxLocations)}); });
    connect(m_settingPanel, &SettingPanel::maxMaterialsChanged, [this](int maxMaterials)
            { m_dataHandler->addFilter(Filter{Filter::FilterType::MATERIALS_MAX, QVariant(maxMaterials)}); });

    m_dataHandler->start();
}
void GeneralDashboard::setUpHeader()
{
    QLabel *appTitleLabel = new QLabel(tr("Water Quality Monitor"));
    appTitleLabel->setAlignment(Qt::AlignHCenter);
    appTitleLabel->setStyleSheet("font-size: 24px; font-weight: bold; margin-bottom: 10px;");
    if (m_mainLayout)
        m_mainLayout->addWidget(appTitleLabel);
}
void GeneralDashboard::setUpSettingPanel()
{
    m_settingPanel = new SettingPanel(this);
    // m_settingPanel->setStyleSheet("border: 1px solid black;");
    if (m_contentLayout)
        // Add the setting panel to the layout at row 1, column 0, and make it span 10 rows and 1 column
        m_contentLayout->addWidget(m_settingPanel);
}
void GeneralDashboard::setUpChartPanel()
{
    m_chartPanel = new OverviewChartWidget(this);
    if (m_contentLayout)
        // Add the chart panel to the layout at row 2, column 0, and make it span 10 rows and 1 column
        m_contentLayout->addWidget(m_chartPanel);
}
void GeneralDashboard::setUpStatusBar()
{
    m_statusBar = new StatusBar(this);
    if (m_mainLayout)
        // Add the status bar to the layout at row 3, column 0 and make it span 1 row and all columns
        m_mainLayout->addWidget(m_statusBar);
    m_statusBar->hide();
}
void GeneralDashboard::setUpPreventDialog()
{
    m_dialog = new QDialog(this);
    m_dialog->setWindowTitle("Processing...");
    m_dialog->setModal(true);
    m_dialog->setFixedSize(300, 100);
    QVBoxLayout *layout = new QVBoxLayout;
    m_dialog->setLayout(layout);
    QLabel *label = new QLabel("Please wait while we process the data...");
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);
}
void GeneralDashboard::onProcessing(int percent)
{
    // LOGD(QString("percent %1").arg(percent));
    if (percent == HIDE_PROGRESS_VALUE)
    {
        m_dialog->close();
        m_statusBar->hide();
    }
    else
    {
        if (!m_dialog->isVisible())
            m_dialog->show();
        m_statusBar->show();
        m_statusBar->setValue(percent);
    }
}
void GeneralDashboard::setProcessingMessage(QString message)
{
    LOGD(message);
    if (!message.isEmpty())
        m_statusBar->setText(message);
    else
        m_statusBar->setText("Processing...");
}
