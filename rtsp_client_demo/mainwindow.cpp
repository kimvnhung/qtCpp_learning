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

    m_rtspClientManager = new RtspClientManager;
    connect(m_rtspClientManager,&RtspClientManager::frameAvailable,this, &MainWindow::onFrameAvailable);

    ui->setupUi(this);

    // Alterando o titulo do QMainWindow
    setWindowTitle("Test RTSP with QMediaPlayer");

    QGridLayout *layout = new QGridLayout;

    QLabel *titleLable = new QLabel(this);
    titleLable->setText("Demonstrate add QLabel OK");
    VideoWidget *videoWidget = new VideoWidget(this);
    layout->addWidget(videoWidget);

    Handle handleID = m_rtspClientManager->addClient("rtsp://admin:abcd1234@192.168.1.250:554/1/1?transmode=unicast&profile=vaom");

    m_players.insert(handleID,videoWidget);

    // Cria um QWudget que irá receber o layout com os VideoWidgets
    QWidget *win = new QWidget();
    win->setLayout(layout);
    setCentralWidget(win);

}

void MainWindow::onFrameAvailable(Handle handleId,QImage frame)
{
    qDebug()<<__FUNCTION__<<__LINE__;
    VideoWidget *label = m_players.value(handleId,nullptr);
    if(label != nullptr){
        label->setImage(&frame);
    }
}

MainWindow::~MainWindow()
{

    delete ui;
}
