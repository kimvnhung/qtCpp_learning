#ifndef OVERVIEWCHARTWIDGET_H
#define OVERVIEWCHARTWIDGET_H

#include <QWidget>

class QGridLayout;
class ChartHolderBaseWidget;
class QStackedWidget;

class OverviewChartWidget : public QWidget
{
    Q_OBJECT
    using ChartWidget = ChartHolderBaseWidget;
public:
    explicit OverviewChartWidget(QWidget *parent = nullptr);

signals:
    void pageChanged(QString pageName);

public slots:
    void setPreview();
    void setExpanded(int index);

    void onBackButtonClicked();
    void onExpanded();

    void onEnvironmentalLitterIndicatorsChartUpdated(QStringList locations, QStringList materials, QMap<QString,QList<double>> results, double maxValue);
private:
    QStackedWidget *m_stackWidget;
    QGridLayout *m_previewLayout;

    QList<ChartWidget*> *m_listChart;
    QWidget *m_expandedWidget;

    bool m_isPreview;
    int m_interactiveIndex;

private:
    void initializeUi();

    void setUpChart();
    void setUpExpandedWidget();

    void setPreviewMode();
    void setExpandedMode();
};

#endif // OVERVIEWCHARTWIDGET_H
