#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QVideoWidget>
#include <QMediaPlayer>
#include <QGridLayout>
#include <QNetworkRequest>
#include <QThread>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    backgroundThread = new QThread;
    videoGenerator = new VideoWidgetGenerator;
    ui->setupUi(this);

    // Alterando o titulo do QMainWindow
    setWindowTitle("Test RTSP with QMediaPlayer");

    // Criando 4 QVideoWidget
    // for (int i = 0; i < NUM_PLAYER; i++) {
    //     QVideoWidget *videoWidget = new QVideoWidget(this);
    //     mutex.lock();
    //     videoWidgets.append(videoWidget);
    //     mutex.unlock();
    // }

    // // Criando 4 QMediaPlayer
    // for (int i = 0; i < NUM_PLAYER; i++) {
    //     QMediaPlayer *player = new QMediaPlayer(this);
    //     mutex.lock();
    //     mediaPlayers.append(player);
    //     mutex.unlock();
    // }

    // // Criando um QGridLayout de 4x4
    gridLayout = new QGridLayout;
    // int maxColumnCount = qSqrt(NUM_PLAYER);
    // for (int row = 0; row < maxColumnCount; row++) {
    //     for(int column = 0; column < maxColumnCount;column++){
    //         mutex.lock();
    //         layout->addWidget(videoWidgets.at(row*maxColumnCount+column),row,column,1,1);
    //         mutex.unlock();
    //     }
    // }

    // int count = layout->count();

    // // Cria um QWudget que irá receber o layout com os VideoWidgets
    QWidget *win = new QWidget();
    win->setLayout(gridLayout);
    setCentralWidget(win);

    // const QUrl url1 = QUrl("rtsp://admin:abcd1234@192.168.1.250:554/1/1?transmode=unicast&profile=vaom");
    // // QMediaPlayer -> QVideoWidget
    // for (int row = 0; row < maxColumnCount; row++) {
    //     for(int column = 0; column < maxColumnCount;column++){
    //         mutex.lock();
    //         mediaPlayers.at(row*maxColumnCount+column)->setVideoOutput(videoWidgets.at(row*maxColumnCount+column));
    //         mediaPlayers.at(row*maxColumnCount+column)->setSource(url1);
    //         mutex.unlock();
    //     }
    // }


    // // Links de RTSP e Videos

    // // const QUrl url2 = QUrl("rtsp://admin:admin@192.168.1.1/22");
    // // const QUrl url3 = QUrl("rtsp://192.168.1.2/1/stream1");
    // // const QUrl url4 = QUrl("rtsp://192.168.1.3/1/stream");

    // // O NetworkRequest para os links
    // // Adicionandos os links para o conteudo

    // for (int i = 0; i < NUM_PLAYER; i++) {
    //     mutex.lock();
    //     connect(backgroundThread,&QThread::started,mediaPlayers.at(i),&QMediaPlayer::play,Qt::QueuedConnection);
    //     connect(backgroundThread,&QThread::finished,mediaPlayers.at(i),&QMediaPlayer::stop);
    //     mediaPlayers.at(i)->moveToThread(backgroundThread);
    //     mutex.unlock();
    // }

    connect(this, &MainWindow::prepareVideo, videoGenerator, &VideoWidgetGenerator::prepareVideoWidget);
    connect(backgroundThread, &QThread::finished, videoGenerator, &VideoWidgetGenerator::stop);
    connect(videoGenerator, &VideoWidgetGenerator::addNewVideoWidget,this,&MainWindow::on_addNewVideoWidget);

    videoGenerator->moveToThread(backgroundThread);
    backgroundThread->start();
    QTimer::singleShot(1000,[this]{
        for (int i = 0; i < VideoWidgetGenerator::NUM_PLAYER; i++) {
            emit prepareVideo(new QVideoWidget(this));
        }
    });
}

int row,colum = 0;
void MainWindow::on_addNewVideoWidget(QVideoWidget *video)
{
    qDebug()<<__FILE__<<__LINE__<<__FUNCTION__;
    int maxColumnCount = qSqrt(VideoWidgetGenerator::NUM_PLAYER);
    mutex.lock();
    if(row >= maxColumnCount){
        row = 0;
        colum++;
    }
    gridLayout->addWidget(video,row++,colum,1,1);
    mutex.unlock();
}

MainWindow::~MainWindow()
{
    if(backgroundThread != nullptr){
        if(backgroundThread->isRunning()){
            backgroundThread->quit();
        }
        backgroundThread->deleteLater();
        backgroundThread = nullptr;
        videoGenerator->deleteLater();
    }
    delete ui;
}
