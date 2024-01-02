#include "videowidgetgenerator.h"

#include <QVideoWidget>


int VideoWidgetGenerator::NUM_PLAYER = 16;

VideoWidgetGenerator::VideoWidgetGenerator(QObject *parent)
    : QObject{parent}
{
    threadPool = new QThreadPool(this);
    threadPool->setMaxThreadCount(NUM_PLAYER+4);
}

void VideoWidgetGenerator::prepareVideoWidget(QVideoWidget *videoWidget)
{
    const QUrl url1 = QUrl("rtsp://admin:abcd1234@192.168.1.250:554/1/1?transmode=unicast&profile=vaom");
    threadPool->start(QRunnable::create([this,url1,videoWidget]{
        qDebug()<<"onCreateNewVideoWidget "<<QThread::currentThreadId();
        QMediaPlayer *player = new QMediaPlayer;
        qDebug()<<__FUNCTION__<<__LINE__;
        player->setVideoOutput(videoWidget);
        qDebug()<<__FUNCTION__<<__LINE__;
        player->setSource(url1);
        qDebug()<<__FUNCTION__<<__LINE__;
        emit addNewVideoWidget(videoWidget);
        mutex.lock();
        videoWidgets.append(videoWidget);
        mediaPlayers.append(player);
        mutex.unlock();
        player->play();
        qDebug()<<"finishCreateNewVideWidget"<<QThread::currentThreadId();
    }));
}

void VideoWidgetGenerator::stop()
{
    qDebug()<<"VideoWidgetGenerator stop";
    for (int i = 0; i < NUM_PLAYER; i++) {
        mutex.lock();
        videoWidgets.at(i)->deleteLater();
        mediaPlayers.at(i)->deleteLater();
        mutex.unlock();
    }
}

