#ifndef AVPLAYERMANAGER_H
#define AVPLAYERMANAGER_H

#include <QtAVPlayer/qavplayer.h>
#include <QObject>

class AVPlayerManager : public QObject
{
    Q_OBJECT
public:
    explicit AVPlayerManager(QObject *parent = nullptr,QStringList urls = QStringList());
    ~AVPlayerManager();

signals:
    void videoFrame(int index, const QAVVideoFrame &frame);
public slots:
    void play();
private:
    QList<QAVPlayer*> m_players;
};
#endif // AVPLAYERMANAGER_H
