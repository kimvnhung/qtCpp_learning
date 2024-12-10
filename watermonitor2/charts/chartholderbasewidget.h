#ifndef CHARTHOLDERBASEWIDGET_H
#define CHARTHOLDERBASEWIDGET_H

#define COMPLIANCE_CHART "Compliance Chart"
#define ENVIRONMENT_LITTER_INDICATOR_CHART "Environment Litter Indicator Chart"
#define FLOURINATED_COMPOUNDS_CHART "Flourinated Compounds Chart"
#define POLLUTANT_OVERVIEW_CHART "Pollutant Overview Chart"
#define POPS_CHART "POPs Chart"
#define RAW_DATA_PAGE "Raw Data Page"
#define PREVIEW_MODE_NAME "Preview Mode"

#include <QWidget>

class QChartView;
class QGridLayout;
class QPropertyAnimation;

class ChartHolderBaseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChartHolderBaseWidget(QWidget *parent = nullptr);

    enum ViewMode {
        UNDEFINED,
        PREVIEW,
        EXPANDED
    };

    void setMode(ViewMode mode);
    ViewMode mode() const;
    QChartView *chartWidget() const ;
    virtual QString summary() const = 0;
    virtual QString name() const = 0;

signals:
    void expanded();
    void collapsed();

protected:
    virtual void setUpChart() = 0;
    void initializeUi();

    QGridLayout *mainLayout() const;
    QWidget *previewWidget() const;
    QWidget *expandedWidget() const;

    void setChartWidget(QChartView *chartContent);



    // Handle extended functionality
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

private:
    QGridLayout *m_mainLayout;

    QWidget *m_previewWidget = nullptr;
    QWidget *m_expandedWidget = nullptr;

    QChartView *m_chartContent = nullptr;

    ViewMode m_viewMode = UNDEFINED;

    bool m_isHovered;

private:
    void setUpPreviewWidget();

    void setUpExpandedWidget();

    void switchMode();
};

#endif // CHARTHOLDERBASEWIDGET_H
