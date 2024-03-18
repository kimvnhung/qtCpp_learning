#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "customwidget.h"
#include "avplayermanager.h"

#include <QGridLayout>

int NUM_CAM_SIZE = 3;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    CustomWidget *widget = new CustomWidget(NUM_CAM_SIZE);
    QStringList urls;
    urls << QStringLiteral("rtsp://admin:abcd1234@14.241.58.36");
    urls << QStringLiteral("rtsp://admin:abcd1234@14.241.37.207");
    urls << QStringLiteral("rtsp://admin:abcd1234@14.241.65.20:554/");
    urls << QStringLiteral("rtsp://admin:abcd1234@14.241.65.40:554/cam/realmonitor?channel=1&subtype=0&unicast=true&proto=Onvif");
    urls << QStringLiteral("rtsp://admin:abcd1234@113.160.155.237/cam/realmonitor?channel=0");
    urls << QStringLiteral("rtsp://admin:abcd1234@116.97.240.172:554/Streaming/Channels/1501/");
    urls << QStringLiteral("rtsp://admin:abcd1234@116.97.240.172:554/Streaming/Channels/301/");
    urls << QStringLiteral("rtsp://admin:abcd1234@116.97.240.172:554/Streaming/Channels/501/");
    urls << QStringLiteral("rtsp://admin:abcd1234@116.97.240.172:554/Streaming/Channels/601/");
    urls << QStringLiteral("rtsp://admin:abcd1234@116.97.240.172:554/Streaming/Channels/701/");
    urls << QStringLiteral("rtsp://admin:abcd1234@116.97.240.172:554/Streaming/Channels/801/");
    urls << QStringLiteral("rtsp://admin:abcd1234@116.97.240.172:554/Streaming/Channels/901/");
    urls << QStringLiteral("rtsp://admin:abcd1234@116.97.240.172:554/Streaming/Channels/1101/");
    urls << QStringLiteral("rtsp://admin:abcd1234@14.241.35.248");
    urls << QStringLiteral("rtsp://admin:abcd1234@14.241.65.20");
    urls << QStringLiteral("rtsp://admin:abcd1234@14.241.65.109");

    AVPlayerManager *manager = new AVPlayerManager(nullptr, urls.mid(0, NUM_CAM_SIZE * NUM_CAM_SIZE));
    connect(manager,&AVPlayerManager::videoFrame,widget,&CustomWidget::videoFrame);

    QGridLayout* layout = new QGridLayout;
    layout->addWidget(widget);
    ui->centralwidget->setLayout(layout);
    // widget->play("rtsp://admin:abcd1234@14.241.65.40:554/cam/realmonitor?channel=1&subtype=0&unicast=true&proto=Onvif");
    manager->play();
}

MainWindow::~MainWindow()
{
    delete ui;
}
