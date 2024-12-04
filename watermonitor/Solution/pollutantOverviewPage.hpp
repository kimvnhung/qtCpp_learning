#pragma once

#include <QChartView>
#include <QWidget>

class QPushButton;

/**
 * @brief Template for an information page.
 */
class PollutantOverviewPage : public QWidget
{
    Q_OBJECT

public:
    explicit PollutantOverviewPage(QWidget *parent = nullptr);

    QWidget *getChart() const;
signals:
    void goBack();
public slots:
    void updateChart(QStringList materials, QList<int> counts, QList<double> avgs, int maxCount, double maxAvg);

    void initChart();
private:
    QPushButton *backButton;

    QWidget *placeholder;
    QChartView* chartHolder = nullptr;


};
