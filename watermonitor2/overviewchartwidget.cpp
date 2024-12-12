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
#include <QPlainTextEdit>
#include "charts/popschart.h"
#include "charts/rawdatapage.h"
#include "common.h"
#define INTERACTIVE_INDEX_EMPTY -1
OverviewChartWidget::OverviewChartWidget(QWidget *parent)
    : QWidget{parent}, m_interactiveIndex(INTERACTIVE_INDEX_EMPTY)
{
    initializeUi();
}
// private
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
    m_previewLayout->setRowStretch(0, 1);    // Row 0 stretches proportionally
    m_previewLayout->setRowStretch(1, 1);    // Row 1 stretches proportionally
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
    LOGD(QString("Children count: %1").arg(buttonLayout->count()));
    layout->addLayout(buttonLayout);

    // Add a summary text
    QHBoxLayout *contentLayout = new QHBoxLayout;

    QVBoxLayout *summaryLayout = new QVBoxLayout;
    QLabel *summaryText = new QLabel("Summary:");
    QPlainTextEdit *editText = new QPlainTextEdit;
    editText->setFixedWidth(200);
    editText->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    editText->setReadOnly(true);
    summaryLayout->addWidget(summaryText);
    summaryLayout->addWidget(editText);
    summaryLayout->addStretch();

    contentLayout->addLayout(summaryLayout);

    layout->addLayout(contentLayout);

    // m_expandedWidget->setStyleSheet("background-color: #000000;");

    m_stackWidget->addWidget(m_expandedWidget);
}
void OverviewChartWidget::onBackButtonClicked()
{
    if (m_interactiveIndex != INTERACTIVE_INDEX_EMPTY)
    {
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

    auto chart = qobject_cast<ChartWidget *>(sender());
    chart->setMode(ChartWidget::EXPANDED);

    auto contentLayout = dynamic_cast<QHBoxLayout *>(m_expandedWidget->layout()->itemAt(1)->layout());
    if (contentLayout == nullptr)
        LOGD("No content layout");
    else
    {
        contentLayout->insertWidget(0,chart->chartWidget());
        contentLayout->layout()->itemAt(0)->widget()->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        auto summaryLayout = dynamic_cast<QVBoxLayout *>(contentLayout->itemAt(1)->layout());
        if (summaryLayout == nullptr)
            LOGD("No summary layout");
        else
        {
            auto summaryText = dynamic_cast<QPlainTextEdit *>(summaryLayout->itemAt(1)->widget());
            if (summaryText == nullptr)
                LOGD("No summary text");
            else
                summaryText->setPlainText(chart->summary());
        }
    }

    // Find the interactive index
    for (int i = 0; i < m_listChart->size(); ++i)
    {
        if (m_listChart->at(i) == chart)
        {
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
    auto pollutantOverviewChart = dynamic_cast<PollutantOverviewChart *>(chart);
    if (pollutantOverviewChart)
    {
        pollutantOverviewChart->updateChart(pollutants, valuesByMonth, maxValue);
    }
}
void OverviewChartWidget::onEnvironmentalLitterIndicatorsChartUpdated(QStringList locations, QStringList materials, QMap<QString, QList<double>> results, double maxValue)
{
    LOG();
    auto chart = m_listChart->at(2);
    auto environmentalLitterChart = dynamic_cast<EnvironmentalLitterIndicatorsChart *>(chart);
    if (environmentalLitterChart)
    {
        environmentalLitterChart->updateChart(locations, materials, results, maxValue);
    }
}
void OverviewChartWidget::onFlourinatedCompoundsChartUpdated(QStringList locations, QList<double> lats, QList<double> lons, QList<double> values, double minLat, double minLon, double maxLat, double maxLon, double maxValue)
{
    LOG();
    auto chart = m_listChart->at(3);
    auto flourinatedCompoundsChart = dynamic_cast<FlourinatedCompoundsChart *>(chart);
    if (flourinatedCompoundsChart)
        flourinatedCompoundsChart->updateChart(locations, lats, lons, values, minLat, minLon, maxLat, maxLon, maxValue);
}
void OverviewChartWidget::onPOPsChartUpdated(QStringList locations, QList<QList<double>> valuesByMonth, double maxValue)
{
    LOG();
    auto chart = m_listChart->at(4);
    auto popschart = dynamic_cast<POPsChart *>(chart);
    if (popschart)
        popschart->updateChart(locations, valuesByMonth, maxValue);
}

void OverviewChartWidget::onComplianceChartUpdated(int trueCount, int falseCount)
{
    LOG();
    auto chart = m_listChart->at(1);
    auto complianceChart = dynamic_cast<ComplianceChart *>(chart);
    if (complianceChart)
        complianceChart->updateChart(trueCount, falseCount);
}

void OverviewChartWidget::onRawDataUpdated(std::vector<Water> data)
{
    LOG();
    auto chart = m_listChart->at(5);
    auto rawDataPage = dynamic_cast<RawDataPage *>(chart);
    if (rawDataPage)
    {
        rawDataPage->updateChart(data);
    }
}
void OverviewChartWidget::setUpChart()
{
    m_listChart = new QList<ChartWidget *>;
    m_listChart->append(new PollutantOverviewChart(this));
    m_listChart->append(new ComplianceChart(this));
    m_listChart->append(new EnvironmentalLitterIndicatorsChart(this));
    m_listChart->append(new FlourinatedCompoundsChart(this));
    m_listChart->append(new POPsChart(this));
    m_listChart->append(new RawDataPage(this));
    for (int i = 0; i < m_listChart->size(); ++i)
    {
        auto chart = m_listChart->at(i);
        connect(chart, &ChartWidget::expanded, this, &OverviewChartWidget::onExpanded);
        m_previewLayout->addWidget(chart, i / 2, i % 2);
    }
}


void OverviewChartWidget::setPreviewMode()
{
    auto chart = m_listChart->at(m_interactiveIndex);
    chart->setMode(ChartWidget::PREVIEW);
    m_previewLayout->addWidget(chart, m_interactiveIndex / 2, m_interactiveIndex % 2);
}
