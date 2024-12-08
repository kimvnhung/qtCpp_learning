#include "overviewchartwidget.h"
#include "charts/compliancechart.h"
#include "charts/environmentallitterindicatorschart.h"
#include "charts/flourinatedcompoundschart.h"
#include "charts/pollutantoverviewchart.h"

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include "charts/popschart.h"
#include "common.h"

#define INTERACTIVE_INDEX_EMPTY -1

OverviewChartWidget::OverviewChartWidget(QWidget *parent)
    : QWidget{parent}
    , m_isPreview(true)
    , m_interactiveIndex(INTERACTIVE_INDEX_EMPTY)
{
    initializeUi();
}

//private
void OverviewChartWidget::initializeUi()
{
    // Set for always expanding
    // setStyleSheet("OverviewChartWidget { border: 1px solid black; }");
    // setFixedSize(1280, 600);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);

    m_stackWidget = new QStackedWidget(this);
    QWidget *previewWidget = new QWidget;

    m_previewLayout = new QGridLayout;
    // Set stretch factors for rows and columns
    m_previewLayout->setRowStretch(0, 1);  // Row 0 stretches proportionally
    m_previewLayout->setRowStretch(1, 1);  // Row 1 stretches proportionally
    m_previewLayout->setColumnStretch(0, 1); // Column 0 stretches proportionally
    m_previewLayout->setColumnStretch(1, 1); // Column 1 stretches proportionally

    // Remove spacing and margins (optional)
    m_previewLayout->setHorizontalSpacing(0);
    m_previewLayout->setVerticalSpacing(0);
    m_previewLayout->setContentsMargins(0, 0, 0, 0);

    setUpChart();
    previewWidget->setLayout(m_previewLayout);
    m_stackWidget->addWidget(previewWidget);

    setUpExpandedWidget();
    layout->addWidget(m_stackWidget);
}

void OverviewChartWidget::setUpExpandedWidget()
{
    m_expandedWidget = new QWidget;
    // m_expandedWidget->setStyleSheet("background-color: #000000;");
    m_expandedWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *layout = new QVBoxLayout;
    m_expandedWidget->setLayout(layout);

    QPushButton *backButton = new QPushButton("Back");
    backButton->setMinimumSize(100, 30);
    connect(backButton, &QPushButton::clicked, this, &OverviewChartWidget::onBackButtonClicked);
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(backButton);

    layout->addLayout(buttonLayout);

    m_stackWidget->addWidget(m_expandedWidget);
}

void OverviewChartWidget::onBackButtonClicked()
{
    m_stackWidget->setCurrentIndex(0);
    setPreviewMode();
}

void OverviewChartWidget::onExpanded()
{
    // Embbed the chart into the expanded widget
    auto chart = qobject_cast<ChartWidget*>(sender());
    chart->setMode(ChartWidget::EXPANDED);
    static_cast<QVBoxLayout*>(m_expandedWidget->layout())->addWidget(chart);

    //Find the interactive index
    for (int i = 0; i < m_listChart->size(); ++i) {
        if (m_listChart->at(i) == chart) {
            m_interactiveIndex = i;
            break;
        }
    }

    m_stackWidget->setCurrentIndex(1);
}

void OverviewChartWidget::setUpChart()
{
    m_listChart = new QList<ChartWidget*>;
    m_listChart->append(new PollutantOverviewChart(this));
    m_listChart->append(new ComplianceChart(this));
    m_listChart->append(new EnvironmentalLitterIndicatorsChart(this));
    m_listChart->append(new FlourinatedCompoundsChart(this));
    m_listChart->append(new POPsChart(this));

    for (int i = 0; i < m_listChart->size(); ++i) {
        auto chart = m_listChart->at(i);
        connect(chart, &ChartWidget::expanded, this, &OverviewChartWidget::onExpanded);
    }

    if(m_isPreview)
        setPreviewMode();
    else
        setExpandedMode();
}

void OverviewChartWidget::setPreview()
{
    m_isPreview = true;
    setPreviewMode();
}

void OverviewChartWidget::setExpanded(int index)
{
    m_isPreview = false;
    setExpandedMode();
    m_listChart->at(index)->setMode(ChartWidget::EXPANDED);
}

void OverviewChartWidget::setPreviewMode()
{
    for (int i = 0; i < m_listChart->size(); i++) {

    }
}

void OverviewChartWidget::setExpandedMode()
{
    for (auto chart : *m_listChart)
        chart->setMode(ChartWidget::EXPANDED);
}
