#pragma once
#include <QWidget>
#include <QStackedWidget>

class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QWidget;
class DataHandler;
class CreateFilters;
class QLabel;

class DashboardPage : public QWidget {
  Q_OBJECT

public:
    DashboardPage(QWidget* parent = nullptr, QStackedWidget* pageStack = nullptr
                , QWidget *pollutantsOverviewChart = nullptr
                , QWidget *complianceDashboardChart = nullptr
                , QWidget *fluorinatedCompoundsChart = nullptr
                , QWidget *environmentalLitterIndicatorsChart = nullptr
                , QWidget *popChart = nullptr
                , QWidget *geographicalHotspotsChart = nullptr
                , DataHandler* dataHandler=nullptr);

    QWidget *createCard(const QString &title,QWidget* cardWidget, QWidget *parent, std::function<void()> onClickCallback);

    CreateFilters* timePeriodFilter;

    enum FilterType {
      PAST_30_DAYS,
      PAST_3_MONTHS,
      PAST_6_MONTHS,
      YEAR_TILL_DATE,
      PAST_YEAR,
      ALL_TIME
  };

  FilterType currentFilter() const;

private:
    QStackedWidget* pageStack;
    QLabel *statusLabel;
    QGridLayout *cardLayout = nullptr;
    QWidget *pollutantsOverviewChart;
    QWidget *complianceDashboardChart;
    QWidget *fluorinatedCompoundsChart;
    QWidget *environmentalLitterIndicatorsChart;
    QWidget *popChart;
    QWidget *geographicalHotspotsChart;
    FilterType curFilter = FilterType::ALL_TIME;
    QWidget *dashboardCards;



public slots:
    void updateStatus(const QString &message);
    void reloadCharts();
  
signals:
    void loadCSV();    
}; 

