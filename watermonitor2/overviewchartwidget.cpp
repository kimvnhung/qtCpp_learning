#include "overviewchartwidget.h"
#include "charts/compliancechart.h"
#include "charts/environmentallitterindicatorschart.h"
#include "charts/flourinatedcompoundschart.h"
#include "charts/pollutantoverviewchart.h"

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QChartView>
#include "charts/popschart.h"
#include "charts/rawdatapage.h"
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

    QLabel *sumaryText = new QLabel("Summary");
    sumaryText->setStyleSheet("font-size: 20px; font-weight: bold;");
    sumaryText->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    sumaryText->setMinimumSize(100, 30);
    sumaryText->setAlignment(Qt::AlignCenter);

    buttonLayout->addStretch();
    buttonLayout->addWidget(sumaryText);
    buttonLayout->addStretch();
    buttonLayout->addWidget(backButton);

    LOGD(QString("Children count: %1").arg(buttonLayout->count()));

    layout->addLayout(buttonLayout);

    m_stackWidget->addWidget(m_expandedWidget);
}

void OverviewChartWidget::onBackButtonClicked()
{
    if(m_interactiveIndex != INTERACTIVE_INDEX_EMPTY) {
        setPreviewMode();
        m_interactiveIndex = INTERACTIVE_INDEX_EMPTY;
    }
    else
        LOGD("Interactive index is empty");
    m_stackWidget->setCurrentIndex(0);
}

void OverviewChartWidget::onExpanded()
{
    // Embbed the chart into the expanded widget
    // Remove widget at index 1 of m_expandedWidget
    auto item = m_expandedWidget->layout()->takeAt(1);
    if(item == nullptr)
        LOGD("No item to take");

    auto chart = qobject_cast<ChartWidget*>(sender());
    chart->setMode(ChartWidget::EXPANDED);
    static_cast<QVBoxLayout*>(m_expandedWidget->layout())->addWidget(chart->chartWidget());

    auto buttonLayout = m_expandedWidget->layout()->itemAt(0);
    if(buttonLayout == nullptr)
        LOGD("No button layout found");
    else {
        LOGD(QString("Children count: %1").arg(buttonLayout->layout()->count()));

        auto summaryText = qobject_cast<QLabel*>(buttonLayout->layout()->itemAt(1)->widget());
        if(summaryText)
            summaryText->setText(chart->summary());
        else
            LOGD("No summary text found");
    }


    //Find the interactive index
    for (int i = 0; i < m_listChart->size(); ++i) {
        if (m_listChart->at(i) == chart) {
            m_interactiveIndex = i;
            break;
        }
    }

    m_stackWidget->setCurrentIndex(1);
}

void OverviewChartWidget::onPollutantOverviewChartUpdated(QStringList pollutants, QList<QList<double>> valuesByMonth, double maxValue)
{
    LOG();
    auto chart = m_listChart->at(0);
    auto pollutantOverviewChart = dynamic_cast<PollutantOverviewChart*>(chart);
    if (pollutantOverviewChart) {
        pollutantOverviewChart->updateChart(pollutants, valuesByMonth, maxValue);
    }
}

void OverviewChartWidget::onEnvironmentalLitterIndicatorsChartUpdated(QStringList locations, QStringList materials, QMap<QString, QList<double>> results, double maxValue)
{
    LOG();
    auto chart = m_listChart->at(2);
    auto environmentalLitterChart = dynamic_cast<EnvironmentalLitterIndicatorsChart*>(chart);
    if (environmentalLitterChart) {
        environmentalLitterChart->updateChart(locations, materials, results, maxValue);
    }
}

void OverviewChartWidget::onRawDataUpdated(std::vector<Water> data)
{
    LOG();
    auto chart = m_listChart->at(5);
    auto rawDataPage = dynamic_cast<RawDataPage*>(chart);
    if (rawDataPage) {
        rawDataPage->updateChart(data);
    }
}

void OverviewChartWidget::setUpChart()
{
    m_listChart = new QList<ChartWidget*>;
    m_listChart->append(new PollutantOverviewChart(this));
    m_listChart->append(new ComplianceChart(this));
    m_listChart->append(new EnvironmentalLitterIndicatorsChart(this));
    m_listChart->append(new FlourinatedCompoundsChart(this));
    m_listChart->append(new POPsChart(this));
    m_listChart->append(new RawDataPage(this));

    for (int i = 0; i < m_listChart->size(); ++i) {
        auto chart = m_listChart->at(i);
        connect(chart, &ChartWidget::expanded, this, &OverviewChartWidget::onExpanded);
        m_previewLayout->addWidget(chart, i / 2, i % 2);
    }
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
    auto chart = m_listChart->at(m_interactiveIndex);
    chart->setMode(ChartWidget::PREVIEW);
    m_previewLayout->addWidget(chart, m_interactiveIndex / 2, m_interactiveIndex % 2);
}

void OverviewChartWidget::setExpandedMode()
{
    for (auto chart : *m_listChart)
        chart->setMode(ChartWidget::EXPANDED);
}
