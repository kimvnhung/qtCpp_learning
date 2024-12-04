#pragma once


#include <QWidget>

class QPushButton;
class QChartView;
class QPieSeries;
/**
 * @brief Template for an information page.
 */
class ComplianceDashboardPage : public QWidget
{
    Q_OBJECT

public:
    ComplianceDashboardPage(QWidget *parent = nullptr);

public slots:
    void updateChart(int trueCount, int falseCount);

signals:
    void goBack();

private:
    QPushButton *backButton;

    QChartView *chartHolder;
    QPieSeries *pieSeries;

    void initChart();
};
