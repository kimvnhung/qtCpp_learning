#include "generaldashboard.h"

#include <QLabel>

GeneralDashboard::GeneralDashboard(QWidget *parent)
    : QWidget{parent}
{
    initializeUi();
}

//private
void GeneralDashboard::initializeUi()
{
    m_mainLayout = new QGridLayout;
    setUpHeader();
    setUpSettingPanel();
    setUpChartPanel();
    setUpStatusBar();

    setLayout(m_mainLayout);
    setMinimumSize(800,600);
}

void GeneralDashboard::setUpHeader()
{
    QLabel *appTitleLabel = new QLabel(tr("Water Quality Monitor"));
    appTitleLabel->setAlignment(Qt::AlignHCenter);
    appTitleLabel->setStyleSheet("font-size: 24px; font-weight: bold; margin-bottom: 10px;");

    if(m_mainLayout)
        // Add the app title label to the layout at row 0, column 0 and make it span 1 row and all columns
        m_mainLayout->addWidget(appTitleLabel, 0, 0,1,-1);
}

void GeneralDashboard::setUpSettingPanel()
{
    m_settingPanel = new SettingPanel(this);
    if(m_mainLayout)
        // Add the setting panel to the layout at row 1, column 0, and make it span 10 rows and 1 column
        m_mainLayout->addWidget(m_settingPanel, 1, 0,10,1);
}

void GeneralDashboard::setUpChartPanel()
{
    m_chartPanel = new OverviewChartWidget(this);
    if(m_mainLayout)
        // Add the chart panel to the layout at row 2, column 0, and make it span 10 rows and 1 column
        m_mainLayout->addWidget(m_chartPanel, 1, 1,10,-1);
}

void GeneralDashboard::setUpStatusBar()
{
    m_statusBar = new StatusBar(this);
    if(m_mainLayout)
        // Add the status bar to the layout at row 3, column 0 and make it span 1 row and all columns
        m_mainLayout->addWidget(m_statusBar, 2, 0, 1, -1);
}
