#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QGridLayout>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_demoWidget = new DemoQuickWidget(this);
    ui->centralwidget->layout()->addWidget(m_demoWidget->widget());
    QTimer::singleShot(1000,[this]{
        m_demoWidget->setText("Hellsadasd");
    });

    // m_sampleView = new SampleView(this);
    // m_bubble = new BubbleToolTip2(this);
    m_timeMarker = new TimeMarker(this);
    // ui->centralwidget->layout()->addWidget(m_sampleView->widget());
    // ui->centralwidget->layout()->addWidget(m_bubble->widget());
    ui->centralwidget->layout()->addWidget(m_timeMarker->widget());
    m_timeMarker->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_increaseSpeedBtn_clicked()
{
    m_sampleView->increaseSpeed();
}


void MainWindow::on_decreaseSpeedBtn_clicked()
{
    m_sampleView->decreaseSpeed();
}


void MainWindow::on_increaseFanBtn_clicked()
{
    m_sampleView->increaseFan();
}


void MainWindow::on_decreaseFanBtn_clicked()
{
    m_sampleView->decreaseFan();
}

