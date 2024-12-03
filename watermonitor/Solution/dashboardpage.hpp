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
  DashboardPage(QWidget *parent = nullptr);
  QWidget *createCard(const QString &title, QWidget *parent, std::function<void()> onClickCallback);

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
};
