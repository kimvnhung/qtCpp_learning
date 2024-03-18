#include "avplayermanager.h"

AVPlayerManager::AVPlayerManager(QObject *parent, QStringList urls)
    : QObject{parent}
{
    for (int i=0;i<urls.size();i++) {
        QAVPlayer *player = new QAVPlayer(this);
        // player->setFilter("scale='min(320,iw)':'min(240,ih)'");
        player->setSource(urls.at(i));
        connect(player, &QAVPlayer::videoFrame,[this,i](const QAVVideoFrame &frame){
            emit videoFrame(i,frame);
        });
        m_players.append(player);
    }
}

AVPlayerManager::~AVPlayerManager()
{

}

void AVPlayerManager::play()
{
    foreach (QAVPlayer* p, m_players) {
        p->play();
    }
}
