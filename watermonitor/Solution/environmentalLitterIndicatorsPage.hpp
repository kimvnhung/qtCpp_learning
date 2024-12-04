#pragma once

#include <QWidget>

class QPushButton;
class QChartView;

/**
 * @brief Template for an information page.
 */
class EnvironmentalLitterIndicatorsPage : public QWidget
{
    Q_OBJECT

public:
    explicit EnvironmentalLitterIndicatorsPage(QWidget *parent = nullptr);

    QWidget* getChart() const;
public slots:
    void updateChart();
    void initChart();
signals:
    void goBack();

private:
    QPushButton *backButton;

    QChartView *chartHolder = nullptr;


};
