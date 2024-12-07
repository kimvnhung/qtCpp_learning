#ifndef OVERVIEWCHARTWIDGET_H
#define OVERVIEWCHARTWIDGET_H

#include <QWidget>

class QGridLayout;
class ChartHolderBaseWidget;



class OverviewChartWidget : public QWidget
{
    Q_OBJECT
    using ChartWidget = ChartHolderBaseWidget;
public:
    explicit OverviewChartWidget(QWidget *parent = nullptr);


private:
    QGridLayout *m_mainLayout;

    QList<ChartWidget*> *m_listChart;

private:
    void initializeUi();

    void setUpChart();

    void setPreviewMode();
    void setExpandedMode();
};

#endif // OVERVIEWCHARTWIDGET_H
