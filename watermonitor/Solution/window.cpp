// COMP2811 Coursework 2 sample solution: main window

#include <QtWidgets>
#include <stdexcept>
#include <iostream>
#include "window.hpp"
#include "stats.hpp"
#include "dashboardpage.hpp"
#include "POPpage.hpp"
#include "pollutantOverviewPage.hpp"
#include "complianceDashboardPage.hpp"
#include "fluorinatedCompoundsPage.hpp"
#include "environmentalLitterIndicatorsPage.hpp"
#include "geographicalHotspotsPage.hpp"
#include "rawDataPage.hpp"
#include "common.hpp"

static const int MIN_WIDTH = 620;

WaterQualityWindow::WaterQualityWindow(QWidget *parent) : QMainWindow(parent), pageStack(new QStackedWidget(this)), statsDialog(nullptr)
{


  dataHandler = new DataHandler(this);
  connect(dataHandler, &DataHandler::handling, this, &WaterQualityWindow::onHandleData);
  connect(dataHandler, &DataHandler::dataReady, [this]() {
      dashboardPage->updateStatus("Data loaded successfully.");
  });

  progressDialog = new QProgressDialog(this);
  progressDialog->setCancelButton(nullptr);
  progressDialog->cancel();
  progressDialog->setWindowModality(Qt::WindowModal);
  progressDialog->setMinimum(0);
  progressDialog->setMaximum(MAX_PROGRESS_VALUE);
  progressDialog->setWindowTitle("Processing...");
  // Apply custom style to increase the progress bar height
  progressDialog->setStyleSheet(
      "QProgressBar {"
      "    min-height: 20px;"  // Adjust this value for height
      "    max-height: 30px;"  // Keep consistent for fixed height
      "    border: 1px solid #000;"
      "    border-radius: 5px;"
      "    text-align: center;"
      "}"
      "QProgressBar::chunk {"
      "    background-color: #5D9CEC;"
      "    width: 20px;"  // Chunk width for progress bar segments
      "}"
      );

  msgBox = new QDialog(this);

  popPage = new POPpage(this);
  connect(dataHandler, &DataHandler::POPDataReady, popPage, &POPpage::updateChart);

  pollutantOverviewPage = new PollutantOverviewPage(this);
  connect(dataHandler, &DataHandler::pollutantOverviewDataReady, pollutantOverviewPage, &PollutantOverviewPage::updateChart);

  complianceDashboardPage = new ComplianceDashboardPage(this);
  connect(dataHandler, &DataHandler::complianceDashboardDataReady, complianceDashboardPage, &ComplianceDashboardPage::updateChart);

  fluorinatedCompoundsPage = new FluorinatedCompoundsPage(this);
  environmentalLIPage = new EnvironmentalLitterIndicatorsPage(this);
  rawDataPage = new RawDataPage(this);
  QGridLayout *layout = rawDataPage->mainLayout;
  layout->addWidget(createDataTable(), 2, 1, 8, 6);
  rawDataPage->setLayout(layout);
  geographicalHotspotPage = new GeographicalHotspotsPage(this,dataHandler);
  connect(geographicalHotspotPage, &GeographicalHotspotsPage::handling, this, &WaterQualityWindow::updateProgress);

    dashboardPage = new DashboardPage(this
                                    ,pollutantOverviewPage->getChart()
                                    ,complianceDashboardPage->getChart()
                                    , fluorinatedCompoundsPage->getChart()
                                    , environmentalLIPage->getChart()
                                    ,popPage->getChart()
                                    ,geographicalHotspotPage->getChart());

  pageStack->addWidget(dashboardPage); // Index 0
  pageStack->addWidget(popPage);         // Index 1
  pageStack->addWidget(pollutantOverviewPage);         // Index 2
  pageStack->addWidget(complianceDashboardPage);         // Index 3
  pageStack->addWidget(fluorinatedCompoundsPage);         // Index 4
  pageStack->addWidget(environmentalLIPage);         // Index 5
  pageStack->addWidget(rawDataPage);         // Index 6
  pageStack->addWidget(geographicalHotspotPage);         // Index 7

  connect(dashboardPage, &DashboardPage::goToPOPpage, this, &WaterQualityWindow::navigateToPOPpage);
  connect(dashboardPage, &DashboardPage::goToPollutantOverviewPage, this, &WaterQualityWindow::navigateToPollutantOverviewPage);
  connect(dashboardPage, &DashboardPage::goToComplianceDashboardPage, this, &WaterQualityWindow::navigateToComplianceDashboardPage);
  connect(dashboardPage, &DashboardPage::goToFluorinatedCompoundsPage, this, &WaterQualityWindow::navigateToFluorinatedCompoundsPage);
  connect(dashboardPage, &DashboardPage::goToEnvironmentalLitterIndicatorsPage, this, &WaterQualityWindow::navigateToEnvironmentalLitterIndicatorsPage);
  connect(dashboardPage, &DashboardPage::goToRawDataPage, this, &WaterQualityWindow::navigateToRawDataPage);
  connect(dashboardPage, &DashboardPage::goToGeographicalHotspotsPage, this, &WaterQualityWindow::navigateToGeographicalHotspotsPage);

  connect(dashboardPage, &DashboardPage::goToGeographicalHotspotsPage, dataHandler, &DataHandler::triggerGeographicalHotspots);
  connect(dashboardPage, &DashboardPage::goToComplianceDashboardPage, dataHandler, &DataHandler::triggerComplianceDashboard);
  connect(dashboardPage, &DashboardPage::goToPollutantOverviewPage, dataHandler, &DataHandler::triggerPollutantOverview);
  connect(dashboardPage, &DashboardPage::goToPOPpage, dataHandler, &DataHandler::triggerPOP);
  connect(dashboardPage, &DashboardPage::goToFluorinatedCompoundsPage, dataHandler, &DataHandler::triggerFluorinatedCompounds);
  connect(dashboardPage, &DashboardPage::goToEnvironmentalLitterIndicatorsPage, dataHandler, &DataHandler::triggerEnvironmentalLitterIndicators);

  connect(dashboardPage, &DashboardPage::goToGeographicalHotspotsPage, geographicalHotspotPage, &GeographicalHotspotsPage::initChart);
  connect(dashboardPage, &DashboardPage::goToComplianceDashboardPage, complianceDashboardPage, &ComplianceDashboardPage::initChart);
  connect(dashboardPage, &DashboardPage::goToPollutantOverviewPage, pollutantOverviewPage, &PollutantOverviewPage::initChart);
  connect(dashboardPage, &DashboardPage::goToPOPpage, popPage, &POPpage::initChart);
  connect(dashboardPage, &DashboardPage::goToFluorinatedCompoundsPage, fluorinatedCompoundsPage, &FluorinatedCompoundsPage::initChart);
  connect(dashboardPage, &DashboardPage::goToEnvironmentalLitterIndicatorsPage, environmentalLIPage, &EnvironmentalLitterIndicatorsPage::initChart);


  connect(dashboardPage, &DashboardPage::loadCSV, this, &WaterQualityWindow::openCSV);
  connect(rawDataPage, &RawDataPage::loadCSV, this, &WaterQualityWindow::openCSV);

  connect(popPage, &POPpage::goBack, this, &WaterQualityWindow::navigateToDashboard);
  connect(pollutantOverviewPage, &PollutantOverviewPage::goBack, this, &WaterQualityWindow::navigateToDashboard);
  connect(complianceDashboardPage, &ComplianceDashboardPage::goBack, this, &WaterQualityWindow::navigateToDashboard);
  connect(fluorinatedCompoundsPage, &FluorinatedCompoundsPage::goBack, this, &WaterQualityWindow::navigateToDashboard);
  connect(environmentalLIPage, &EnvironmentalLitterIndicatorsPage::goBack, this, &WaterQualityWindow::navigateToDashboard);
  connect(rawDataPage, &RawDataPage::goBack, this, &WaterQualityWindow::navigateToDashboard);
  connect(geographicalHotspotPage, &GeographicalHotspotsPage::goBack, this, &WaterQualityWindow::navigateToDashboard);

  setCentralWidget(pageStack);

  createFileSelectors();
  createStatusBar();
  addFileMenu();
  addHelpMenu();

  setMinimumWidth(MIN_WIDTH);
  resize(1200, 800);
  setWindowTitle("Water Quality Monitor Tool");

    init();

      dataHandler->start();
}

WaterQualityWindow::~WaterQualityWindow()
{
    dataHandler->stop();
    dataHandler->quit();
    dataHandler->wait();
    delete dataHandler;
}

void WaterQualityWindow::init()
{
    LOG();
    // init old csv
    dataLocation = GET_STRING(CSV_FILE_PATH);
    fileInfo->setText(QString("Current file: <kbd>%1</kbd>").arg(QFileInfo(dataLocation).fileName()));
}

void WaterQualityWindow::navigateToDashboard()
{
    dashboardPage->reloadCharts();
  pageStack->setCurrentIndex(0);
}

void WaterQualityWindow::navigateToPOPpage()
{
  pageStack->setCurrentIndex(1);
}

void WaterQualityWindow::navigateToPollutantOverviewPage()
{
  pageStack->setCurrentIndex(2);
}

void WaterQualityWindow::navigateToComplianceDashboardPage()
{
  pageStack->setCurrentIndex(3);
}

void WaterQualityWindow::navigateToFluorinatedCompoundsPage()
{
  pageStack->setCurrentIndex(4);
}

void WaterQualityWindow::navigateToEnvironmentalLitterIndicatorsPage()
{
  pageStack->setCurrentIndex(5);
}

void WaterQualityWindow::navigateToRawDataPage()
{
  pageStack->setCurrentIndex(6);
}

void WaterQualityWindow::navigateToGeographicalHotspotsPage()
{
  pageStack->setCurrentIndex(7);
}

QWidget *WaterQualityWindow::createDataTable()
{
  model = new WaterModel(this,dataHandler);
  QFrame *tableFrame = createFrame();
  QVBoxLayout *tableLayout = new QVBoxLayout(tableFrame);
  table = new QTableView();
  table->setModel(model);

  QFont tableFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
  table->setFont(tableFont);
  tableLayout->addWidget(table);

  return tableFrame;
}

void WaterQualityWindow::createFileSelectors()
{
  QStringList significanceOptions;
  significanceOptions << "significant" << "4.5" << "2.5" << "1.0" << "all";
  significance = new QComboBox();
  significance->addItems(significanceOptions);

  QStringList periodOptions;
  periodOptions << "hour" << "day" << "week" << "month";
  period = new QComboBox();
  period->addItems(periodOptions);
}

void WaterQualityWindow::createToolBar()
{
  QToolBar *toolBar = new QToolBar();

  QLabel *significanceLabel = new QLabel("Significance");
  significanceLabel->setAlignment(Qt::AlignVCenter);
  toolBar->addWidget(significanceLabel);
  toolBar->addWidget(significance);

  QLabel *periodLabel = new QLabel("Period");
  periodLabel->setAlignment(Qt::AlignVCenter);
  toolBar->addWidget(periodLabel);
  toolBar->addWidget(period);

  toolBar->addSeparator();

  loadButton = new QPushButton("Load");
  connect(loadButton, SIGNAL(clicked()), this, SLOT(openCSV()));
  toolBar->addWidget(loadButton);

  addToolBar(Qt::LeftToolBarArea, toolBar);
}
void WaterQualityWindow::createStatusBar()
{
  fileInfo = new QLabel("Current file: <none>");
  QStatusBar *status = statusBar();
  status->addWidget(fileInfo);
}

void WaterQualityWindow::addHelpMenu()
{
  QAction *aboutAction = new QAction("&About", this);
  connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

  QAction *aboutQtAction = new QAction("About &Qt", this);
  connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

  QMenu *helpMenu = menuBar()->addMenu("&Help");
  helpMenu->addAction(aboutAction);
  helpMenu->addAction(aboutQtAction);
}

void WaterQualityWindow::addFileMenu()
{
  QAction *openFileAction = new QAction("Open &File", this);
  openFileAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_O));
  connect(openFileAction, SIGNAL(triggered()), this, SLOT(setDataLocation()));

  QAction *closeAction = new QAction("Quit", this);
  closeAction->setShortcut(QKeySequence::Close);
  connect(closeAction, SIGNAL(triggered()), this, SLOT(close()));

  QMenu *fileMenu = menuBar()->addMenu("&File");
  fileMenu->addAction(openFileAction);
  fileMenu->addAction(closeAction);
}
void WaterQualityWindow::setDataLocation()
{
  LOG();
  QString filePath = QFileDialog::getOpenFileName(
      this, "Select CSV File", ".",
      "CSV Files (*.csv);;All Files (*)");

  LOGD(QString("Selected file: %1").arg(filePath));
  if (!filePath.isEmpty())
  {
    dataLocation = filePath;
    SET_VALUE(CSV_FILE_PATH, filePath);

    fileInfo->setText(QString("Current file: <kbd>%1</kbd>").arg(QFileInfo(filePath).fileName()));
  }
}
void WaterQualityWindow::openCSV()
{
  if (dataLocation.isEmpty())
  {
    QMessageBox::critical(this, "File Error",
                          "No file has been selected!\n\n"
                          "Please select a file via the File menu.");
    return;
  }

  try
  {
    updateProgress(SHOW_PROGESS_VALUE);
      dataHandler->loadData(dataLocation.toStdString(),dashboardPage->currentFilter());
  }
  catch (const std::exception &error)
  {
    QMessageBox::critical(this, "CSV File Error", error.what());
    return;
  }

  fileInfo->setText(QString("Current file: <kbd>%1</kbd>").arg(QFileInfo(dataLocation).fileName()));
  table->resizeColumnsToContents();
}

void WaterQualityWindow::about()
{
  QMessageBox::about(this, "About Water Quality Monitor Tool",
                     "Water Quality Monitor Tool displays and analyzes water quality data loaded from"
                     "a CSV file produced by ...\n\n"
                     "(c) 2024 Best Group");
}

void WaterQualityWindow::onHandleData(int percent)
{
    LOGD(QString("percent: %1").arg(percent));
    if(percent != HIDE_PROGRESS_VALUE)
    {

        updateProgress(percent, "Importing data...", "Please wait...");
    }
    else
    {
        updateProgress(HIDE_PROGRESS_VALUE);
    }
}

void WaterQualityWindow::updateProgress(int value, QString title, QString label)
{
    LOGD(QString("value: %1").arg(value));
    if(title.isEmpty())
    {
        title = "Processing...";
    }

    if(label.isEmpty())
    {
        label = "Please wait...";
    }
    progressDialog->setLabelText(label);
    progressDialog->setWindowTitle(title);
    if (value == SHOW_PROGESS_VALUE)
    {
        progressDialog->show();
    }
    else if (value == HIDE_PROGRESS_VALUE)
    {
        progressDialog->hide();
    }
    else
    {
        progressDialog->setValue(value);
    }
}
