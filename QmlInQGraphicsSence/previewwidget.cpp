#include "previewwidget.h"

PreviewWidget::PreviewWidget(int x, int y, QGraphicsItem *parent)
    : QGraphicsVideoItem{parent}
    , GraphItem{x,y}
{
    m_player = new QMediaPlayer{static_cast<QGraphicsVideoItem*>(this)};
    m_player->setVideoOutput(static_cast<QGraphicsVideoItem*>(this));
    m_player->setSource(QUrl("rtsp://admin:admin123@192.168.1.88/live/ch0"));
    m_player->play();
}

QRectF PreviewWidget::boundingRect() const
{
    QRectF rect = this->rect();
    QRectF result = QRectF(rect.center(),QSizeF(500,500));
    return result;
}

PreviewWidget::~PreviewWidget()
{
    if(m_player != NULL){
        if( m_player->isPlaying()){
            m_player->stop();
        }
        m_player->deleteLater();
    }
}
