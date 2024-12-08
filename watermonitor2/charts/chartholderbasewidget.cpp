#include "chartholderbasewidget.h"


#include "../common.h"
#include <QGridLayout>
#include <QLabel>
#include <QChartView>
#include <QPropertyAnimation>

#define PREVIEW_WIDTH 400
#define PREVIEW_HEIGHT 200

#define EXPANDED_WIDTH 900
#define EXPANDED_HEIGHT 500

#define DEFAULT_COLOR QColor(255, 255, 255,0)
// Grey background when hovered
#define HOVER_COLOR QColor(220, 220, 220)


ChartHolderBaseWidget::ChartHolderBaseWidget(QWidget *parent)
    : QWidget{parent}
    , m_viewMode{UNDEFINED}
    , m_isHovered{false}
{
}

void ChartHolderBaseWidget::setMode(ViewMode mode)
{
    LOGD(QString("mode %1").arg(mode));
    if(m_viewMode == mode)
        return;

    m_viewMode = mode;
    switchMode();
}

void ChartHolderBaseWidget::initializeUi()
{
    LOG();
    m_mainLayout = new QGridLayout;
    setUpChart();
    setLayout(m_mainLayout);

    setMode(PREVIEW);
}

void ChartHolderBaseWidget::setUpPreviewWidget()
{
    LOG();
    // Set Size Policy
    setBaseSize(PREVIEW_WIDTH, PREVIEW_HEIGHT);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    if(m_previewWidget)
    {
        if(m_chartContent)
            m_previewWidget->layout()->addWidget(m_chartContent);
    }
    else{
        m_previewWidget = new QWidget;
        m_previewWidget->setStyleSheet("background-color: #ffffff;");
        QVBoxLayout *layout = new QVBoxLayout;
        m_previewWidget->setLayout(layout);
        if(m_chartContent)
            layout->addWidget(m_chartContent);
    }

    acceptClickEvents(m_previewWidget,false);
}

void ChartHolderBaseWidget::setUpExpandedWidget()
{
    // Set Size Policy
    // setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // m_expandedWidget = new QWidget;
    // m_expandedWidget->setStyleSheet("background-color: #ffffff;");
    // QVBoxLayout *layout = new QVBoxLayout;
    // m_expandedWidget->setLayout(layout);
    // if(m_chartContent)
    //     layout->addWidget(m_chartContent);
}

void ChartHolderBaseWidget::switchMode()
{
    LOGD(QString("m_isPreview %1").arg(m_viewMode));
    // Clean up the existing widget
    // Clean on the main layout

    if(m_viewMode == PREVIEW)
    {
        setFixedSize(PREVIEW_WIDTH, PREVIEW_HEIGHT);
        setUpPreviewWidget();
        m_mainLayout->addWidget(m_previewWidget);
        LOGD(QString("m_previewWidgetSize %1x%2").arg(m_previewWidget->width()).arg(m_previewWidget->height()));
    }
}

QGridLayout *ChartHolderBaseWidget::mainLayout() const
{
    return m_mainLayout;
}

QWidget *ChartHolderBaseWidget::previewWidget() const
{
    return m_previewWidget;
}

QWidget *ChartHolderBaseWidget::expandedWidget() const
{
    return m_expandedWidget;
}

void ChartHolderBaseWidget::setChartWidget(QChartView *chartContent)
{
    m_chartContent = chartContent;
}

QChartView *ChartHolderBaseWidget::chartWidget() const
{
    return m_chartContent;
}

void ChartHolderBaseWidget::enterEvent(QEnterEvent *event)
{
    // Set the target size for expansion (PREVIEW_WIDTH*1.2, PREVIEW_HEIGHT*1.2)
    // setFixedSize(PREVIEW_WIDTH*1.2, PREVIEW_HEIGHT*1.2);
    m_isHovered = true;
    update();
    raise();
    QWidget::enterEvent(event);
}

void ChartHolderBaseWidget::leaveEvent(QEvent *event)
{
    // setFixedSize(PREVIEW_WIDTH, PREVIEW_HEIGHT); // Reset the size to the original size
    m_isHovered = false;
    update();
    QWidget::leaveEvent(event);
}

void ChartHolderBaseWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    if (m_isHovered) {
        // Set the background color when hovered (highlight color)
        painter.fillRect(rect(), HOVER_COLOR); // Light green background when hovered
    } else {
        // Default background color
        painter.fillRect(rect(), DEFAULT_COLOR); // White background when not hovered
    }

    // Continue with the default painting
    QWidget::paintEvent(event);
}

void ChartHolderBaseWidget::mousePressEvent(QMouseEvent *event)
{
    if (m_viewMode == PREVIEW)
        emit expanded();
}

void ChartHolderBaseWidget::resizeEvent(QResizeEvent *event)
{
    // LOGD(QString("size %1x%2").arg(width()).arg(height()));
    QWidget::resizeEvent(event);
}
