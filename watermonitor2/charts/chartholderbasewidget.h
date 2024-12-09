#ifndef CHARTHOLDERBASEWIDGET_H
#define CHARTHOLDERBASEWIDGET_H

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
    QChartView *chartWidget() const ;
    virtual QString summary() const = 0;

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
