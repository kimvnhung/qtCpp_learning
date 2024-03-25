#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "qopenglskyboxwidget.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->openGLWidget = new QOpenGLSkyboxWidget(
        ":/images/front.png",
        ":/images/back.png",
        ":/images/top.png",
        ":/images/bottom.png",
        ":/images/left.png",
        ":/images/right.png",
        this
    );
}

MainWindow::~MainWindow()
{
    delete ui;
}
