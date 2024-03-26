#ifndef FFMPEGPLAYER_H
#define FFMPEGPLAYER_H

#include "framequeue.h"

#include <QObject>
#include <QThread>

class FfmpegPlayer : public QThread
{
    Q_OBJECT
public:
    explicit FfmpegPlayer(FrameQueue *queue, QObject *parent = nullptr);

protected:
    void run() override;

private:
    QString m_url;
    FrameQueue *queue;

    void realDecoded();
    void fakeDecoded();
};

#endif // FFMPEGPLAYER_H
