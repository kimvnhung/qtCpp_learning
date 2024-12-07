#include "chartholderbasewidget.h"

#include <QGridLayout>

ChartHolderBaseWidget::ChartHolderBaseWidget(QWidget *parent)
    : QWidget{parent}
    , m_isPreview{true}
{
    initializeUi();
}

void ChartHolderBaseWidget::setMode(bool isPreview)
{
    m_isPreview = isPreview;
    switchMode();
}

void ChartHolderBaseWidget::initializeUi()
{
    m_mainLayout = new QGridLayout;

    setUpPreviewWidget();
    setUpExpandedWidget();

    setLayout(m_mainLayout);
}

void ChartHolderBaseWidget::setUpPreviewWidget()
{
    // Set Size Policy
    setBaseSize(400,200);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void ChartHolderBaseWidget::setUpExpandedWidget()
{
    // Set Size Policy
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void ChartHolderBaseWidget::switchMode()
{
    if (m_isPreview) {
        setUpPreviewWidget();
    } else {
        setUpExpandedWidget();
    }
}


