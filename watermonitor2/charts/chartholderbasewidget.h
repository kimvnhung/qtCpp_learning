#ifndef CHARTHOLDERBASEWIDGET_H
#define CHARTHOLDERBASEWIDGET_H

#include <QWidget>

class QChartView;
class QGridLayout;

class ChartHolderBaseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChartHolderBaseWidget(QWidget *parent = nullptr);

    void setMode(bool isPreview);

signals:
    void expanded();
    void collapsed();

private:
    QGridLayout *m_mainLayout;

    QWidget *m_previewWidget;
    QWidget *m_expandedWidget;

    QChartView *m_chartContent = nullptr;

    bool m_isPreview;

private:
    void initializeUi();

    void setUpPreviewWidget();

    void setUpExpandedWidget();

    void switchMode();
};

#endif // CHARTHOLDERBASEWIDGET_H
