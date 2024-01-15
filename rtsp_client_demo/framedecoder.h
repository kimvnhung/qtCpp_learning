#ifndef FRAMEDECODER_H
#define FRAMEDECODER_H

#include "framequeue.h"
#include "libde265/de265.h"

#include <QObject>
#include <QRunnable>

class FrameDecoder:public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit FrameDecoder(QObject *parent = nullptr, FrameQueue *frameQueue = NULL);
    ~FrameDecoder();

    void run() override;

    de265_decoder_context* ctx();
    const de265_image* img();

signals:
    void decodeDone(Handle handleId, QImage *image);
private:
    FrameQueue *frameQueue = NULL;
    class Private;
    QScopedPointer<Private> d;
};

#endif // FRAMEDECODER_H
