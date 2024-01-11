#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "imagewindow.h"

#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(new ImageWindow);

    ui->centralwidget->setLayout(layout);
}

MainWindow::~MainWindow()
{
    delete ui;
}
