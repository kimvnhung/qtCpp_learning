#pragma once

#include <QWidget>
#include <QStackedWidget>

class QPushButton;
class QChartView;
/**
 * @brief Template for an information page.
 */
class FluorinatedCompoundsPage : public QWidget {
    Q_OBJECT

public:
    explicit FluorinatedCompoundsPage(QWidget* parent = nullptr, QStackedWidget* pageStack = nullptr);
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
