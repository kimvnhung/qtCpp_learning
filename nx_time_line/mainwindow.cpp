#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "demoquickwidget.h"

#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // m_demoWidget = new DemoQuickWidget(this);
    // ui->centralwidget->layout()->addWidget(m_demoWidget->widget());

    m_sampleView = new SampleView(this);
    ui->centralwidget->layout()->addWidget(m_sampleView->widget());
}

MainWindow::~MainWindow()
{
    delete ui;
}
