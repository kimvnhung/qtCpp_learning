#pragma once

#include <QWidget>
#include <QStackedWidget>

#include "dataHandler.hpp"

#include <QChartView>
#include <QScatterSeries>
#include <QTableWidget>
class QPushButton;

/**
 * @brief Template for an information page.
 */
class GeographicalHotspotsPage: public QWidget {
    Q_OBJECT

public:
    explicit GeographicalHotspotsPage(QWidget* parent, QStackedWidget* pageStack, DataHandler *dataHandler);
    QWidget *getChart() const;
    void initChart();

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
    QWidget* placeholder = nullptr;

    DataHandler *dataHandler;

    QTableWidget *heatMap;

};
