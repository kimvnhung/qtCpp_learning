#pragma once

#include <QWidget>
#include <QStackedWidget>
#include <QChartView>


class QPushButton;

/**
 * @brief Template for an information page.
 */
class PollutantOverviewPage : public QWidget {
    Q_OBJECT

public:
    explicit PollutantOverviewPage(QWidget* parent = nullptr, QStackedWidget* pageStack = nullptr);
    QWidget *getChart() const;
    void initChart();

signals:
    void goBack();

public slots:
    void updateChart(QStringList materials, QList<int> counts, QList<double> avgs, int maxCount, double maxAvg);
    

private:
    QPushButton *backButton;
    QWidget *placeholder;
    QChartView* chartHolder = nullptr;


};