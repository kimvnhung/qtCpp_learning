// COMP2811 Coursework 2 sample solution: main window

#include <QtWidgets>
#include <QPushButton>
#include <QStackedWidget>
#include <QMainWindow>
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
#include "ui_elements.hpp"
#include "common.hpp"

static const int MIN_WIDTH = 620;


WaterQualityWindow::WaterQualityWindow(QWidget* parent)
  : QMainWindow(parent)
{
  pageStack = new QStackedWidget(this);
  setCentralWidget(pageStack);
  createStatusBar();
  initialisePages();
  addFileMenu();
  addHelpMenu();
  addViewMenu();

  setMinimumWidth(MIN_WIDTH);
  resize(800, 600);
  setWindowTitle("Water Quality Monitor Tool");
}

void WaterQualityWindow::initialisePages() {

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

  POPPage = new POPpage(this, pageStack);
  pollutantOverviewPage = new PollutantOverviewPage(this, pageStack);
  complianceDashboardPage = new ComplianceDashboardPage(this, pageStack);
  fluorinatedCompoundsPage = new FluorinatedCompoundsPage(this, pageStack);
  environmentalLitterIndicatorsPage = new EnvironmentalLitterIndicatorsPage(this, pageStack);
  rawDataPage = new RawDataPage(this, pageStack);
  geographicalHotspotsPage = new GeographicalHotspotsPage(this, pageStack, dataHandler);

  QGridLayout* layout =  rawDataPage->mainLayout;
  layout->addWidget(createDataTable(), 2, 1, 8, 6);
  rawDataPage->setLayout(layout);

  // Connect to data handler - add other pages to data handler if loadign graphs
  // pages not included here are filled with dummy data
  connect(dataHandler, &DataHandler::POPDataReady, POPPage, &POPpage::updateChart);
  connect(dataHandler, &DataHandler::pollutantOverviewDataReady, pollutantOverviewPage, &PollutantOverviewPage::updateChart);
  connect(dataHandler, &DataHandler::complianceDashboardDataReady, complianceDashboardPage, &ComplianceDashboardPage::updateChart);
  connect(geographicalHotspotsPage, &GeographicalHotspotsPage::handling, this, &WaterQualityWindow::updateProgress);

  dashboardPage = new DashboardPage(this, pageStack
                                    ,pollutantOverviewPage->getChart()
                                    ,complianceDashboardPage->getChart()
                                    ,fluorinatedCompoundsPage->getChart()
                                    ,environmentalLitterIndicatorsPage->getChart()
                                    ,POPPage->getChart()
                                    ,geographicalHotspotsPage->getChart()
                                    ,dataHandler);

  // Add pages to the pageStack
  pageStack->addWidget(dashboardPage);               // Index 0
  pageStack->addWidget(POPPage);                     // Index 1
  pageStack->addWidget(pollutantOverviewPage);       // Index 2
  pageStack->addWidget(complianceDashboardPage);    // Index 3
  pageStack->addWidget(fluorinatedCompoundsPage);    // Index 4
  pageStack->addWidget(environmentalLitterIndicatorsPage); // Index 5
  pageStack->addWidget(rawDataPage);                 // Index 6
  pageStack->addWidget(geographicalHotspotsPage);    // Index 7

  // Connect CSV loading signals to respective pages
  connect(dashboardPage, &DashboardPage::loadCSV, this, &WaterQualityWindow::openCSV);
  connect(rawDataPage, &RawDataPage::loadCSV, this, &WaterQualityWindow::openCSV);

  connect(dataHandler, &DataHandler::chartDataReady, dashboardPage, &DashboardPage::reloadCharts);


  // Connects filter buttons to functions
  connect(environmentalLitterIndicatorsPage->customFilters, &CreateFilters::filterSelected, this, [this](const QString& buttonText) 
  {
    createFilteredView(buttonText);
  });
  connect(dashboardPage->timePeriodFilter, &CreateFilters::filterSelected, this, [this](const QString& buttonText)
  {
    createFilteredView(buttonText);
  });
  connect(rawDataPage->timePeriodFilter, &CreateFilters::filterSelected, this, [this](const QString& buttonText) {
    createFilteredView(buttonText);
  });

  // Connect goBack signals to dashboard
  connect(POPPage, &POPpage::goBack, [this]() {
    pageStack->setCurrentIndex(0);
  });
  connect(pollutantOverviewPage, &PollutantOverviewPage::goBack, [this]() {
    pageStack->setCurrentIndex(0);
  });
  connect(complianceDashboardPage, &ComplianceDashboardPage::goBack, [this]() {
    pageStack->setCurrentIndex(0);
  });
  connect(fluorinatedCompoundsPage, &FluorinatedCompoundsPage::goBack, [this]() {
    pageStack->setCurrentIndex(0);
  });
  connect(environmentalLitterIndicatorsPage, &EnvironmentalLitterIndicatorsPage::goBack, [this]() {
    pageStack->setCurrentIndex(0);
  });
  connect(rawDataPage, &RawDataPage::goBack, [this]() {
    pageStack->setCurrentIndex(0);
  });
  connect(geographicalHotspotsPage, &GeographicalHotspotsPage::goBack, [this]() {
    pageStack->setCurrentIndex(0);
  });

  // Set the default page
  pageStack->setCurrentIndex(0);

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

// Light and dark mode
void WaterQualityWindow::switchTheme(const QString& theme)
{
  QString qssFile = (theme == "dark") ? ":/resources/dark.qss" : ":/resources/light.qss";
  QFile file(qssFile);
  if (file.open(QFile::ReadOnly))
  {
    QString stylesheet = file.readAll();
    qApp->setStyleSheet(stylesheet);
  }
}

void WaterQualityWindow::addViewMenu()
{
  QMenu* viewMenu = menuBar()->addMenu("View");
  QAction* darkModeAction = new QAction("Dark Mode", this);
  QAction* lightModeAction = new QAction("Light Mode", this);

  connect(darkModeAction, &QAction::triggered, [this]() { switchTheme("dark"); });
  connect(lightModeAction, &QAction::triggered, [this]() { switchTheme("light"); });

  viewMenu->addAction(darkModeAction);
  viewMenu->addAction(lightModeAction);
}


QWidget* WaterQualityWindow::createDataTable()
{
  model = new WaterModel(this, dataHandler);
  QFrame* tableFrame = createFrame();
  QVBoxLayout* tableLayout = new QVBoxLayout(tableFrame);
  table = new QTableView();

  // createProxyTableView();

  QFont tableFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
  table->setFont(tableFont);
  tableLayout->addWidget(table);

  return tableFrame;
}


void WaterQualityWindow::createProxyTableView() {
  proxyModel = new QSortFilterProxyModel(this);
  proxyModel->setSourceModel(model);
  proxyModel->setDynamicSortFilter(true);
  table->setModel(proxyModel);
  table->setSortingEnabled(true);
}

void WaterQualityWindow::createFilteredView(const QString& buttonText) {
  
  if (buttonText == "All Time" || buttonText == "Past 30 Days" || buttonText == "Past 3 Months" || 
      buttonText == "Past 6 Months" || buttonText == "Past 1 Year") 
      {
        filterDatasetByTimePeriod(buttonText);
        table->setModel(proxyModel);
        proxyModel->sort(0, Qt::AscendingOrder);
      }   
  // For other filter implementation, create if statement for all related filters categories,
  // and then call the relevant function. 
}

void WaterQualityWindow::filterDatasetByTimePeriod(const QString& buttonText) {
  if (buttonText == "All Time") {
    // No changes to model
    proxyModel->setFilterFixedString("");
    return;
  }

  QDateTime now = QDateTime::currentDateTime();
  QDateTime filterDate;

  // Sets filter date based on the button text
  if (buttonText == "Past 30 Days") {
    filterDate = now.addDays(-30);
  }
  else if (buttonText == "Past 3 Months") {
    filterDate = now.addMonths(-3);
  }
  else if (buttonText == "Past 6 Months") {
    filterDate = now.addMonths(-6);
  }

  else if (buttonText == "Past 1 Year") {
    filterDate = now.addYears(-1);
  }

  // Custom implementation of QSortFilterProxyModel to filter by the date
  class TimeFilterProxyModel : public QSortFilterProxyModel {
  protected:
    // Takes in the the source model and filters out relavent rows.
    bool filterAcceptsRow(
      int sourceRow, const QModelIndex &sourceParent) const override 
      {
        // Gets time column index and corresponding value from the source model
        QModelIndex timeIndex = sourceModel()->index(sourceRow, 2, sourceParent); 
        QString timeStr = sourceModel()->data(timeIndex).toString();

        // Converts time string to QDateTime
        QDateTime recordTime = QDateTime::fromString(timeStr, Qt::ISODate);

        // Checks if the time on the record is greater than or equal to the filter date.
        // return True, means the row will be included in the proxy model.
        return recordTime >= filterDate;
    }

  public:
    TimeFilterProxyModel(QObject* parent = nullptr) : QSortFilterProxyModel(parent) {}
    void setFilterDate(const QDateTime& date) { filterDate = date; }

  private:
      QDateTime filterDate;
  };

  // Replace previous filter with new time filtering proxy
  if (proxyModel) {
    delete proxyModel;
  }
  
  // Updates proxy model
  auto timeFilterProxy = new TimeFilterProxyModel(this);
  timeFilterProxy->setSourceModel(model);
  timeFilterProxy->setFilterDate(filterDate);    
  proxyModel =  timeFilterProxy;
}


void WaterQualityWindow::createStatusBar()
{
    fileInfo = new QLabel("Current file: <none>");
    QStatusBar* status = statusBar();
    status->addWidget(fileInfo);
}


void WaterQualityWindow::addHelpMenu()
{
    QAction* aboutAction = new QAction("&About", this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    QAction* aboutQtAction = new QAction("About &Qt", this);
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    QMenu* helpMenu = menuBar()->addMenu("&Help");
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);
}


void WaterQualityWindow::addFileMenu()
{
    QAction* openFileAction = new QAction("Open &File", this);
    openFileAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_O));
    connect(openFileAction, SIGNAL(triggered()), this, SLOT(setDataLocation()));

    QAction* closeAction = new QAction("Quit", this);
    closeAction->setShortcut(QKeySequence::Close);
    connect(closeAction, SIGNAL(triggered()), this, SLOT(close()));

    QMenu* fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction(openFileAction);
    fileMenu->addAction(closeAction);
}


void WaterQualityWindow::setDataLocation()
{
    QString filePath = QFileDialog::getOpenFileName(
        this, "Select CSV File", ".",
        "CSV Files (*.csv);;All Files (*)");

    if (!filePath.isEmpty()) {
        dataLocation = filePath;
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

