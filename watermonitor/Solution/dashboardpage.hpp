#pragma once
#include <QWidget>

class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QWidget;

class DashboardPage : public QWidget {
  Q_OBJECT

  public:
    DashboardPage(QWidget* parent = nullptr);
    QWidget* createCard(const QString& title, QWidget* parent, std::function<void()> onClickCallback);

  signals:
    void goToPOPpage();
    void goToPollutantOverviewPage();
    void goToComplianceDashboardPage();
    void goToFluorinatedCompoundsPage();
    void goToEnvironmentalLitterIndicatorsPage();
    void goToRawDataPage();
    void goToGeographicalHotspotsPage();
    void loadCSV();    

}; 

