#include "framedecoder.h"
#include "framequeue.h"

#include <libde265/image.h>

#include <QImage>
#include <QDebug>

class FrameDecoder::Private
{
public:
    Private()
    {
        ctx = de265_new_decoder();
        de265_start_worker_threads(ctx, 4); // start 4 background threads
        mNextBuffer = 0;
        mFrameCount = 0;
    }

    de265_decoder_context* ctx;
    const de265_image* img = NULL;
    int    mNextBuffer;
    int    mFrameCount;

    bool   mPlayingVideo;
    bool   mVideoEnded;
    bool   mSingleStep;
    QImage mImgBuffers[2];

};

const de265_image* FrameDecoder::img()
{
    return d->img;
}

de265_decoder_context* FrameDecoder::ctx()
{
    return d->ctx;
}

FrameDecoder::FrameDecoder(QObject *parent, FrameQueue *frameQueue):
    QObject(parent),
    frameQueue(frameQueue),
    d(new Private())
{

}

FrameDecoder::~FrameDecoder()
{

}

void FrameDecoder::run()
{
    while (true) {
        if(frameQueue == NULL){
            break;
        }

        if(d->img){
            d->img = NULL;
            de265_release_next_picture(d->ctx);
        }

        d->img = de265_peek_next_picture(d->ctx);
        int offset = 0;
        int dataSize = 0;
        Frame frame;
        while (d->img==NULL)
        {
            // mutex.unlock();
            int more=1;
            de265_error err = de265_decode(d->ctx, &more);
            // mutex.lock();
            if(dataSize == 0){
                if(frameQueue->isEmpty()){
                    frameQueue->waitForNotEmpty();
                }

                frame = frameQueue->remove();

                dataSize = frame.dataSize;
                offset = 0;
            }
            qDebug()<<"dataSize : "<<dataSize<<" offset : "<<offset<<" err : "<<err;
            if (more && err == DE265_OK) {
                // try again to get picture

                d->img = de265_peek_next_picture(d->ctx);
            }
            else if (more && err == DE265_ERROR_WAITING_FOR_INPUT_DATA) {
                uint8_t buf[4096];
                int buf_size = qMin(4096,dataSize);
                std::memcpy(buf,frame.data+offset,buf_size);
                int err = de265_push_data(d->ctx,buf,buf_size ,0,0);
                offset += buf_size;
                dataSize -= buf_size;
            }
            else if (!more)
            {
                d->mVideoEnded=true;
                d->mPlayingVideo=false; // TODO: send signal back
                break;
            }
        }


        // show one decoded picture

        if (d->img) {
            if (d->mFrameCount==0) {
                d->mImgBuffers[0] = QImage(QSize(d->img->get_width(),d->img->get_height()), QImage::Format_RGB32);
                d->mImgBuffers[1] = QImage(QSize(d->img->get_width(),d->img->get_height()), QImage::Format_RGB32);
            }

            // --- convert to RGB (or generate a black image if video image is disabled) ---

            QImage* qimg = &d->mImgBuffers[d->mNextBuffer];
            uchar* ptr = qimg->bits();
            int bpl = qimg->bytesPerLine();



            emit decodeDone(frame.m_handle,qimg);
            d->mNextBuffer = 1-d->mNextBuffer;
            d->mFrameCount++;

            if (d->mSingleStep) {
                d->mSingleStep=false;
                d->mPlayingVideo=false;
            }
        }

        // mutex.unlock();
    }
}


