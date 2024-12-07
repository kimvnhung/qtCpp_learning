#pragma once

#include <QWidget>
#include <QStackedWidget>

class QPushButton;
class CreateFilters;
class QChartView;

/**
 * @brief Template for an information page.
 */
class EnvironmentalLitterIndicatorsPage : public QWidget {
    Q_OBJECT

public:
    explicit EnvironmentalLitterIndicatorsPage(QWidget* parent = nullptr, QStackedWidget* pageStack = nullptr);
    QWidget* filterPanel;
    CreateFilters* customFilters;
    QWidget* getChart() const;
    void initChart();

public slots:
    void updateChart();

signals:
    void goBack();

private:
    QPushButton* backButton;
    QChartView *chartHolder = nullptr;
};



