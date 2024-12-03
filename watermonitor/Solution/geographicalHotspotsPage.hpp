#pragma once

#include "dataHandler.hpp"

#include <QWidget>
#include <QChartView>
#include <QScatterSeries>

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

private:
    QPushButton *backButton;
    QChartView *chartView;
    QFrame* placeholder;

    DataHandler *dataHandler;

    void initChart();
    void updateChart();
    QScatterSeries* createHeatMapSeries(const std::vector<std::vector<double>>& data, double min, double max);
    QColor valueToColor(double value, double min, double max);
};
