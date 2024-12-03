// COMP2811 Coursework 2 sample solution: main window

#pragma once

#include <QMainWindow>
#include <QMessageBox>
#include <QProgressDialog>
#include <QStackedWidget>
#include <QThread>
#include "model.hpp"

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

class WaterQualityWindow : public QMainWindow
{
  Q_OBJECT

public:
  WaterQualityWindow(QWidget *parent = nullptr);
  ~WaterQualityWindow();
private:
  void init();

  QWidget *createDataTable();
  void createFileSelectors();
  void createToolBar();
  void createStatusBar();
  void addFileMenu();
  void addHelpMenu();

  void navigateToDashboard();
  void navigateToPOPpage();
  void navigateToPollutantOverviewPage();
  void navigateToComplianceDashboardPage();
  void navigateToFluorinatedCompoundsPage();
  void navigateToEnvironmentalLitterIndicatorsPage();
  void navigateToRawDataPage();
  void navigateToGeographicalHotspotsPage();

  WaterModel *model;     // data model used by table
  QString dataLocation; // location of CSV data files
  QTableView *table;    // table of quake data

  QStackedWidget *pageStack;
  DashboardPage *dashboardPage;
  POPpage *page1;
  PollutantOverviewPage *page2;
  ComplianceDashboardPage *page3;
  FluorinatedCompoundsPage *page4;
  EnvironmentalLitterIndicatorsPage *page5;
  RawDataPage *page6;
  GeographicalHotspotsPage *page7;

  QComboBox *significance;  // selector for quake feed significance level
  QComboBox *period;        // selector for quake feed time period
  QPushButton *loadButton;  // button to load a new CSV file
  QPushButton *statsButton; // button to display dataset stats
  QLabel *fileInfo;         // status bar info on current file
  StatsDialog *statsDialog; // dialog to display stats

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
