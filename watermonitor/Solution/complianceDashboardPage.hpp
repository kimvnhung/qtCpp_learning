#pragma once

#include <QWidget>
#include <QStackedWidget>

class QPushButton;
class QChartView;
class QPieSeries;
/**
 * @brief Template for an information page.
 */
class ComplianceDashboardPage : public QWidget {
    Q_OBJECT

public:
    ComplianceDashboardPage(QWidget* parent = nullptr, QStackedWidget* pageStack = nullptr);
    QWidget* getChart() const;
    void initChart();

public slots:
    void updateChart(int trueCount, int falseCount);
    
signals:
    void goBack();

private:
    QPushButton *backButton;
    QChartView *chartHolder = nullptr;
    QPieSeries *pieSeries;



};
