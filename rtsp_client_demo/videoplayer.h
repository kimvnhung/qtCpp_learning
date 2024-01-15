#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QtGui>

#include "videowidget.h""
#include "videodecoder.h""


class VideoPlayer : public QWidget
{
    Q_OBJECT

public:
    VideoPlayer(const QString& filename);

private:
    VideoWidget* videoWidget;
    QPushButton *startButton;
    QPushButton *stopButton;

    VideoDecoder* mDecoder;
};

#endif // VIDEOPLAYER_H
