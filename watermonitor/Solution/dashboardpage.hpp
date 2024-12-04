#pragma once
#include <QWidget>

class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QWidget;
class QLabel;

class DashboardPage : public QWidget
{
  Q_OBJECT

public:
  DashboardPage(QWidget *parent = nullptr
                  , QWidget *pollutantsOverviewChart = nullptr
                  , QWidget *complianceDashboardChart = nullptr
                 , QWidget *fluorinatedCompoundsChart = nullptr
                  , QWidget *environmentalLitterIndicatorsChart = nullptr
                  , QWidget *popChart = nullptr
                  , QWidget *geographicalHotspotsChart = nullptr);
  QWidget *createCard(const QString &title,QWidget* cardWidget, QWidget *parent, std::function<void()> onClickCallback);

  enum FilterType {
      PAST_30_DAYS,
      PAST_3_MONTHS,
      PAST_6_MONTHS,
      YEAR_TILL_DATE,
      PAST_YEAR,
      ALL_TIME
  };

  FilterType currentFilter() const;

  void reloadCharts();
  public slots:
  void updateStatus(const QString &message);
signals:
  void goToPOPpage();
  void goToPollutantOverviewPage();
  void goToComplianceDashboardPage();
  void goToFluorinatedCompoundsPage();
  void goToEnvironmentalLitterIndicatorsPage();
  void goToRawDataPage();
  void goToGeographicalHotspotsPage();
  void loadCSV();
private:
  QLabel *statusLabel;

    QGridLayout *cardLayout = nullptr;

  QWidget *pollutantsOverviewChart;
  QWidget *complianceDashboardChart;
  QWidget *fluorinatedCompoundsChart;
  QWidget *environmentalLitterIndicatorsChart;
  QWidget *popChart;
  QWidget *geographicalHotspotsChart;

  FilterType curFilter = FilterType::ALL_TIME;

};
