#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QVideoWidget>
#include <QMediaPlayer>
#include <QGridLayout>
#include <QNetworkRequest>
#include <QThread>

int MainWindow::NUM_PLAYER = 16;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    backgroundThread = new QThread;

    ui->setupUi(this);

    // Alterando o titulo do QMainWindow
    setWindowTitle("Test RTSP with QMediaPlayer");

    QGridLayout *layout = new QGridLayout;

    QLabel *titleLable = new QLabel(this);
    titleLable->setText("Demonstrate add QLabel OK");
    showImage = new QLabel(this);
    showImage->setText("Initialized");
    layout->addWidget(titleLable,0,3);
    layout->addWidget(showImage,1,0,5,6);

    // Cria um QWudget que irá receber o layout com os VideoWidgets
    QWidget *win = new QWidget();
    win->setLayout(layout);
    setCentralWidget(win);

    // const QUrl url1 = QUrl("rtsp://admin:abcd1234@192.168.1.250:554/1/1?transmode=unicast&profile=vaom");
    rtspClient = new RtspClient(nullptr,"rtsp://admin:abcd1234@192.168.1.250:554/1/1?transmode=unicast&profile=vaom");
    connect(backgroundThread, &QThread::started, rtspClient, &RtspClient::startOpenRtsp);
    connect(backgroundThread, &QThread::finished, rtspClient, &RtspClient::stopStream);

    backgroundThread->start();
}

void MainWindow::onFrameAvailable(uint8_t *frame)
{
    qDebug()<<__FUNCTION__<<__LINE__;
    // QImage image((uchar*)frame,100,100,QImage::Format::);
    // showImage->setPixmap(QPixmap::fromImage(image));
}

MainWindow::~MainWindow()
{
    if(backgroundThread != nullptr){
        if(backgroundThread->isRunning()){
            backgroundThread->quit();
        }
        backgroundThread->deleteLater();
        backgroundThread = nullptr;
    }
    delete ui;
}
