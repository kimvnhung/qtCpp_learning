#include "customwidget.h"

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QAbstractVideoSurface>
#include <QVideoSurfaceFormat>
#include <QMediaService>
#include <QMediaObject>
#include <QVideoRendererControl>

class VideoRenderer : public QVideoRendererControl
{
public:
    QAbstractVideoSurface *surface() const override
    {
        return m_surface;
    }

    void setSurface(QAbstractVideoSurface *surface) override
    {
        m_surface = surface;
    }

    QAbstractVideoSurface *m_surface = nullptr;
};

class MediaService : public QMediaService
{
public:
    MediaService(VideoRenderer *vr, QObject* parent = nullptr)
        : QMediaService(parent)
        , m_renderer(vr)
    {
    }

    QMediaControl* requestControl(const char *name) override
    {
        if (qstrcmp(name, QVideoRendererControl_iid) == 0)
            return m_renderer;

        return nullptr;
    }

    void releaseControl(QMediaControl *) override
    {
    }

    VideoRenderer *m_renderer = nullptr;
};

class MediaObject : public QMediaObject
{
public:
    explicit MediaObject(VideoRenderer *vr, QObject* parent = nullptr)
        : QMediaObject(parent, new MediaService(vr, parent))
    {
    }
};

class VideoWidget : public QVideoWidget
{
public:
    bool setMediaObject(QMediaObject *object) override
    {
        return QVideoWidget::setMediaObject(object);
    }
};
#else
#include <QGridLayout>
#include <QLabel>
#include <QTimer>
#include <QVideoSink>
#endif

#include <QtAVPlayer/qavplayer.h>



CustomWidget::CustomWidget(int num_size)
{
    QGridLayout *layout = new QGridLayout(this);
    for(int row=0;row<num_size;row++){
        for(int col=0;col<num_size;col++){
            QVideoWidget* newWidget = new QVideoWidget(this);
            m_listWidget.append(newWidget);
            // QLabel* newWidget = new QLabel(this);
            // newWidget->setText(QString("Hello %1,%2").arg(row).arg(col));
            layout->addWidget(newWidget,row,col);
        }
    }

    setLayout(layout);
}

CustomWidget::CustomWidget(const CustomWidget &other){
    // assign(other);
}

CustomWidget& CustomWidget::operator=(const CustomWidget &other){
    if (this != &other) {

    }
    return *this;
}

CustomWidget::CustomWidget(CustomWidget &&other){

}

CustomWidget& CustomWidget::operator=(CustomWidget &&other){
    return *this;
}

CustomWidget::~CustomWidget(){

}

void CustomWidget::print(const char* msg)
{
    qDebug()<<msg;
}

void CustomWidget::play(const char* url)
{
    qDebug()<<__FUNCTION__;
    QMediaPlayer *m_player = new QMediaPlayer(this);
    QVideoWidget *m_videoWidget = new QVideoWidget(this);
    m_videoWidget->resize(400,300);
    m_player->setVideoOutput(m_videoWidget);
    m_listPlayer.append(m_player);
    m_listWidget.append(m_videoWidget);
    layout()->addWidget(m_videoWidget);
    m_player->setSource(QUrl(url));
    m_player->play();
}

void CustomWidget::videoFrame(int index,const QAVVideoFrame &frame)
{
    QVideoFrame videoFrame = frame;
    m_listWidget.at(index)->videoSink()->setVideoFrame(videoFrame);
}
