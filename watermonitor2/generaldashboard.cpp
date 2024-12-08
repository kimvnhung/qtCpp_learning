#include "generaldashboard.h"
#include "common.h"

#include <QDialog>
#include <QLabel>

GeneralDashboard::GeneralDashboard(QWidget *parent)
    : QWidget{parent}
{
    initializeUi();
}

//private
void GeneralDashboard::initializeUi()
{
    m_mainLayout = new QVBoxLayout;
    //Set up the header
    setUpHeader();

    //Set up  the content
    m_contentLayout = new QHBoxLayout;
    setUpSettingPanel();
    m_contentLayout->addStretch();
    setUpChartPanel();
    m_contentLayout->addStretch();
    m_mainLayout->addLayout(m_contentLayout);

    //Set up the status bar
    setUpStatusBar();

    setLayout(m_mainLayout);
    setMinimumSize(1280,720);
}

void GeneralDashboard::setUpHeader()
{
    QLabel *appTitleLabel = new QLabel(tr("Water Quality Monitor"));
    appTitleLabel->setAlignment(Qt::AlignHCenter);

    appTitleLabel->setStyleSheet("font-size: 24px; font-weight: bold; margin-bottom: 10px;");

    if(m_mainLayout)
        m_mainLayout->addWidget(appTitleLabel);
}

void GeneralDashboard::setUpSettingPanel()
{
    m_settingPanel = new SettingPanel(this);
    // m_settingPanel->setStyleSheet("border: 1px solid black;");
    if(m_contentLayout)
        // Add the setting panel to the layout at row 1, column 0, and make it span 10 rows and 1 column
        m_contentLayout->addWidget(m_settingPanel);
}

void GeneralDashboard::setUpChartPanel()
{
    m_chartPanel = new OverviewChartWidget(this);
    if(m_contentLayout)
        // Add the chart panel to the layout at row 2, column 0, and make it span 10 rows and 1 column
        m_contentLayout->addWidget(m_chartPanel);
}

void GeneralDashboard::setUpStatusBar()
{
    m_statusBar = new StatusBar(this);
    if(m_mainLayout)
        // Add the status bar to the layout at row 3, column 0 and make it span 1 row and all columns
        m_mainLayout->addWidget(m_statusBar);

    m_statusBar->hide();
}

void GeneralDashboard::setUpPreventDialog()
{
    m_dialog = new QDialog(this);
    m_dialog->setWindowTitle("Processing...");
    m_dialog->setModal(true);
    m_dialog->setFixedSize(400, 200);

    QVBoxLayout *layout = new QVBoxLayout;
    m_dialog->setLayout(layout);

    QLabel *label = new QLabel("Please wait while we process the data...");
    layout->addWidget(label);
}

void GeneralDashboard::onProcessing(int percent)
{
    if(percent == HIDE_PROGRESS_VALUE)
    {
        m_dialog->close();
        m_statusBar->hide();
    }
    else {
        if(!m_dialog->isVisible())
            m_dialog->show();
        m_statusBar->show();
        m_statusBar->setValue(percent);
    }
}

void GeneralDashboard::setProcessingMessage(QString message)
{
    m_statusBar->setText(message);
}
