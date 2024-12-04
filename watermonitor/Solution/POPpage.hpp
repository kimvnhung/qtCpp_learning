#pragma once

#include <QWidget>
#include "ui_elements.hpp"

class QPushButton;
class QVBoxLayout;
class QLabel;
class QFrame;
class QLineEdit;
class QRect;
class QPushButton;
class QChartView;
class QLineSeries;

/**
 * @brief Template for an information page.
 */
class POPpage : public QWidget
{
    Q_OBJECT
public:
    explicit POPpage(QWidget *parent = nullptr);

    QWidget* getChart() const;

public slots:
    void updateChart(QList<double> values, double max, double min);
    void initChart();
signals:
    void goBack();

private:
    QPushButton *backButton;

    QChartView *chartHolder = nullptr;
    QLineSeries *lineSeries;


};
