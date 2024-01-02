#ifndef VIDEOWIDGETGENERATOR_H
#define VIDEOWIDGETGENERATOR_H

#include <QMediaPlayer>
#include <QMutex>
#include <QObject>
#include <QThreadPool>
#include <QVideoWidget>

class VideoWidgetGenerator : public QObject
{
    Q_OBJECT
public:
    explicit VideoWidgetGenerator(QObject *parent = nullptr);
    static int NUM_PLAYER;
signals:
    void addNewVideoWidget(QVideoWidget *videoWidget);
public slots:
    void prepareVideoWidget(QVideoWidget *widget);
    void stop();
private:

    QMutex mutex;
    QList<QVideoWidget*> videoWidgets;
    QList<QMediaPlayer*> mediaPlayers;
    QThreadPool *threadPool = nullptr;
};

#endif // VIDEOWIDGETGENERATOR_H
