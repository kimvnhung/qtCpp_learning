#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "qopenglskyboxwidget.h"
#include "glvideowidget.h"

#include <QFile>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_openRGBbtn_clicked()
{
    if(ui->openGLWidget != NULL){
        delete ui->openGLWidget;
        ui->openGLWidget = NULL;
    }

    if(ui->openGLWidget == NULL){
        ui->openGLWidget = new GLVideoWidget(this);
        QImage img = QImage(":/images/test.png").convertToFormat(QImage::Format_RGB888);
        dynamic_cast<GLVideoWidget*>(ui->openGLWidget)->setQImageParameters(img.format(),img.width(),img.height(),img.bytesPerLine());
        dynamic_cast<GLVideoWidget*>(ui->openGLWidget)->setImage(img);
        ui->openGLWidget->show();
    }
}


void MainWindow::on_openYUVbtn_clicked()
{
    if(ui->openGLWidget != NULL){
        delete ui->openGLWidget;
        ui->openGLWidget = NULL;
    }

    if(ui->openGLWidget == NULL){
        ui->openGLWidget = new GLVideoWidget(this);
        QFile f(":/images/1280x720.yuv");
        f.open(QIODevice::ReadOnly);
        QByteArray data(f.readAll());
        qDebug("data size: %d", data.size());
        const int w = 1280, h = 720;
        dynamic_cast<GLVideoWidget*>(ui->openGLWidget)->setYUV420pParameters(w,h);
        dynamic_cast<GLVideoWidget*>(ui->openGLWidget)->setFrameData(data);
        ui->openGLWidget->show();
    }
}

