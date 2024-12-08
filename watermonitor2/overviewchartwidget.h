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

public slots:
    void setPreview();
    void setExpanded(int index);

    void onBackButtonClicked();
    void onExpanded();

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
