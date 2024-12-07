#pragma once

#include <QWidget>
#include <QStackedWidget>
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
class POPpage : public QWidget {
    Q_OBJECT

public:
    explicit POPpage(QWidget* parent = nullptr, QStackedWidget* pageStack = nullptr);
    QWidget* getChart() const;
    void initChart();

public slots:
    void updateChart(QList<double> values, double max, double min);
    

signals:
    void goBack();

private:
    QPushButton *backButton;
    QChartView *chartHolder = nullptr;
    QLineSeries *lineSeries;
};
