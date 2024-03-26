#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "qopenglskyboxwidget.h"
#include "glvideowidget.h"

#include <QFile>
#include <QTimer>

bool isInitSize = false;
int FPS = 7;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_queue = new FrameQueue(this);
    m_player = new FfmpegPlayer(m_queue);
    m_player->start();

    m_timer = new QTimer(this);
    m_timer->setInterval(1000/FPS);
    connect(m_timer, &QTimer::timeout,this,&MainWindow::on_handleFrame);
    m_timer->start();
}

MainWindow::~MainWindow()
{
    if(m_player != NULL){
        m_player->stop();
    }
    delete ui;
}

void MainWindow::on_openRGBbtn_clicked()
{
    QImage img;
    if(rand()%2==0){
        img = QImage(":/images/test.png").convertToFormat(QImage::Format_RGB888);
    }else {
        img = QImage(":/images/front.png").convertToFormat(QImage::Format_RGB888);
    }
    ui->openGLWidget->setQImageParameters(img.format(),img.width(),img.height(),img.bytesPerLine());
    ui->openGLWidget->setImage(img);
}


void MainWindow::on_openYUVbtn_clicked()
{
    QString imgPath;
    if(rand()%2==0){
        imgPath = ":/images/1280x720.yuv";
    }else {
        imgPath = ":/images/1280x720_2.yuv";
    }
    QFile f(imgPath);
    f.open(QIODevice::ReadOnly);
    QByteArray data(f.readAll());
    qDebug("data size: %d", data.size());
    const int w = 1280, h = 720;
    if(!isInitSize){
        ui->openGLWidget->setYUV420pParameters(w,h);
        isInitSize = true;
    }

    ui->openGLWidget->setFrameData(data);
}


void MainWindow::on_frameAvailable(char *yuvData, int width, int height,int *stride)
{
    // if(!isInitSize){
    //     ui->openGLWidget->setYUV420pParameters(width,height);
    //     isInitSize = true;
    // }

    // m_frameQueues.enqueue(yuvData);
}


// bool isdisplayed = false;
void MainWindow::on_handleFrame()
{
    if(m_queue->hasFrame()){
        YUVFrame frame = m_queue->popFrame();
        if(!frame.isValid()){
            return;
        }
        if(!isInitSize){
            ui->openGLWidget->setYUV420pParameters(frame.width,frame.height);
            isInitSize = true;
        }
        ui->openGLWidget->setFrameData(frame.data);
        qDebug()<<"Displayed";
    }else{
        qDebug()<<"Frame Queue empty";
    }
}
