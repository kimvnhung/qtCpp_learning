#include "statusbar.h"

#include <QLabel>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QVBoxLayout>

StatusBar::StatusBar(QWidget *parent)
    : QWidget{parent}
{
    initializeUi();
}

void StatusBar::initializeUi()
{
    setMinimumHeight(70);
    m_mainLayout = new QVBoxLayout;
    m_mainLayout->setContentsMargins(0, 0, 0, 0);

    setUpOverlayText();
    setUpProgressBar();


    setLayout(m_mainLayout);
}

void StatusBar::setUpProgressBar()
{
    m_progressBar = new QProgressBar;
    m_progressBar->setStyleSheet(R"(
        QProgressBar {
            min-height: 20px;
            max-height: 30px;
            border: 1px solid #000;
            border-radius: 5px;
            text-align: center;
        }

        QProgressBar::chunk {
            background-color: #05B8CC;
            width: 20px;
        }
)");
    // Configure the progress bar
    m_progressBar->setTextVisible(false); // Hide default text
    m_progressBar->setRange(0, 100);
    m_progressBar->setValue(0); // Initial value

    m_mainLayout->addWidget(m_progressBar);
}

void StatusBar::setUpOverlayText()
{
    m_overlayText = new QLabel;
    m_overlayText->setText("Processing...");
    m_overlayText->setAlignment(Qt::AlignHCenter);

    m_mainLayout->addWidget(m_overlayText);
}
