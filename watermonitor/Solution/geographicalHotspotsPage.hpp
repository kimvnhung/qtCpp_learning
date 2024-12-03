#pragma once

#include "dataHandler.hpp"

#include <QWidget>
#include <QChartView>
#include <QScatterSeries>
#include <QTableWidget>

class QPushButton;

/**
 * @brief Template for an information page.
 */
class GeographicalHotspotsPage : public QWidget
{
    Q_OBJECT

public:
    explicit GeographicalHotspotsPage(QWidget *parent = nullptr, DataHandler *dataHandler = nullptr);

signals:
    void goBack();
    void handling(int percent, QString title, QString message);

public slots:
    void updateHeatMap(QStringList locations, QMap<QString,QList<int>> frequency, int min, int max);
    void onHandling(int percent);

protected:
    // Overridden resizeEvent to catch size changes
    void resizeEvent(QResizeEvent* event) override;

private:
    QPushButton *backButton;
    QChartView *chartView;
    QWidget* placeholder;

    DataHandler *dataHandler;

    QTableWidget *heatMap;
    void initHeatMap();

};
