// COMP2811 Coursework 2 sample solution: main window

#pragma once

#include <QMainWindow>
#include <QMessageBox>
#include <QProgressDialog>
#include <QStackedWidget>
#include <QThread>
#include "model.hpp"
#include "dataHandler.hpp"

class QString;
class QComboBox;
class QLabel;
class QPushButton;
class QTableView;
class StatsDialog;
class DashboardPage;
class POPpage;
class PollutantOverviewPage;
class ComplianceDashboardPage;
class FluorinatedCompoundsPage;
class EnvironmentalLitterIndicatorsPage;
class GeographicalHotspotsPage;
class RawDataPage;
class QSortFilterProxyModel;
class DataHandler;

class WaterQualityWindow: public QMainWindow
{
  Q_OBJECT

  public:
    WaterQualityWindow(QWidget* parent = nullptr);
    ~WaterQualityWindow();
    void switchTheme(const QString& theme);
    WaterModel *model; 
 
  private:
    QWidget* createDataTable();
    void createStatusBar();
    void addFileMenu();
    void addHelpMenu();
    void addViewMenu();
    void createFilteredView(const QString& buttonText);
    void createProxyTableView();
    bool filterAcceptsRow(int sourceRow, const QDateTime& filterDate) const;
    void filterDatasetByTimePeriod(const QString& buttonText);
    void init();

    QString dataLocation;      // location of CSV data files
    QTableView* table;         // table of quake data

    QStackedWidget* pageStack;
    void initialisePages();

    DashboardPage* dashboardPage;
    POPpage* POPPage;
    PollutantOverviewPage* pollutantOverviewPage;
    ComplianceDashboardPage* complianceDashboardPage;
    FluorinatedCompoundsPage* fluorinatedCompoundsPage;
    EnvironmentalLitterIndicatorsPage* environmentalLitterIndicatorsPage;
    RawDataPage* rawDataPage;
    GeographicalHotspotsPage* geographicalHotspotsPage;

    QPushButton* loadButton;   // button to load a new CSV file
    QPushButton* statsButton;  // button to display dataset stats
    QLabel* fileInfo;          // status bar info on current file
    StatsDialog* statsDialog;  // dialog to display stats

    QSortFilterProxyModel* proxyModel;
    QProgressDialog *progressDialog;
    QDialog *msgBox;

  public slots:
    void updateProgress(int value, QString title = "", QString label = "");
    void onHandleData(int percent);

  private:
    DataHandler *dataHandler;

  private slots:
    void openCSV();
    void setDataLocation();
    void about();
};
