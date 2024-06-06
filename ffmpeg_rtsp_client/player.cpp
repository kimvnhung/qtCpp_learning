#include "player.h"
#include "log.h"
#include "models/asyncqueue.h"
#include "models/qavhwdevice_d3d11_p.h"
#include "models/qavhwdevice_p.h"

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <QFuture>
#include <QtConcurrent>
#include <QDateTime>

#define BUFFER_SIZE 1024

class Player::Private
{
public:
    Private(Player *owner)
        :owner{owner}
        , avfCtx{nullptr}
        , avcCtx{nullptr}
        , avCodec{nullptr}
        , avPacket{nullptr}
        , avFrame{nullptr}
        , videoStreamIndex{-1}
        , frameQueue{AsyncQueue<AVFrame*>(owner, BUFFER_SIZE)}
        , videoOutput{nullptr}
        , url{""}
        , options{}
        , threadPool{new QThreadPool(owner)}
        , isTerminate{false}
        , state{StoppedState}
    {
        threadPool->setMaxThreadCount(4);
    }

    Player *owner;

    AVFormatContext *avfCtx;
    AVCodecContext *avcCtx;
    AVCodec *avCodec;
    AVPacket *avPacket;
    AVFrame *avFrame;
    int videoStreamIndex;
    AsyncQueue<AVFrame*> frameQueue;
    GLWidget *videoOutput;

    //preset
    QString url;
    QMap<QString,QString> options;

    QThreadPool *threadPool;
    QFuture<void> loadFuture;
    QFuture<void> demuxerFuture;
    QFuture<void> playerFuture;

    void load();
    void detectHardwareDevice();
    void demux();
    void play();
    void terminate();

    QMutex mutex;

    bool isTerminate;
    State state;
    MediaStatus status;
    ErrorData error;

    void setState(State state, ErrorData error = ErrorData());
    void setMediaStatus(MediaStatus status, ErrorData error = ErrorData());
};

void Player::Private::terminate()
{
    if(isTerminate)
        return;

    isTerminate = true;
    frameQueue.terminate();
    loadFuture.waitForFinished();
    demuxerFuture.waitForFinished();
    playerFuture.waitForFinished();
}

void Player::Private::setMediaStatus(MediaStatus status, ErrorData error)
{
    QMutexLocker locker(&mutex);
    if(this->status == status)
        return;

    this->status = status;
    if(error.error != NoError)
        emit owner->error(error);
}

void Player::Private::setState(State state, ErrorData error)
{
    QMutexLocker locker(&mutex);
    if(this->state == state)
        return;

    this->state = state;
    if(error.error != NoError)
        emit owner->error(error);

    emit owner->stateChanged(state);
}

void Player::Private::load()
{
    DBG("");
    avfCtx = avformat_alloc_context();

    DBG(QString("options : ") << options);
    AVDictionary *avFormatOptions = nullptr;
    for (const auto & key: options.keys())
        av_dict_set(&avFormatOptions, key.toUtf8().constData(), options[key].toUtf8().constData(), 0);

    if (avformat_open_input(&avfCtx, url.toStdString().c_str(), nullptr, &avFormatOptions) != 0) {
        fprintf(stderr, "Error opening RTSP stream\n");
        setState(State::StoppedState, {ResourceError, "Error opening RTSP stream"});
    }

    if (avformat_find_stream_info(avfCtx, nullptr) < 0) {
        fprintf(stderr, "Error finding stream information\n");
        setState(State::StoppedState, {ResourceError, "Error finding stream information"});
    }

    videoStreamIndex = -1;
    for (unsigned int i = 0; i < avfCtx->nb_streams; i++) {
        if (avfCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex = i;
            break;
        }
    }

    if (videoStreamIndex == -1) {
        fprintf(stderr, "Error finding video stream\n");
        setState(State::StoppedState, {ResourceError, "Error finding video stream"});
    }

    avcCtx = avcodec_alloc_context3(nullptr);
    detectHardwareDevice();

    avcodec_parameters_to_context(avcCtx, avfCtx->streams[videoStreamIndex]->codecpar);
    const AVCodec *pCodec = avcodec_find_decoder(avcCtx->codec_id);

    if (pCodec == nullptr) {
        fprintf(stderr, "Error finding codec\n");
        setState(State::StoppedState, {ResourceError, "Error finding codec"});
    }


    if (avcodec_open2(avcCtx, pCodec, NULL) < 0) {
        fprintf(stderr, "Error opening codec\n");
        setState(State::StoppedState, {ResourceError, "Error opening codec"});
    }

    setState(State::PlayingState);
}

void Player::Private::detectHardwareDevice()
{
    // AVHWDeviceType type = AV_HWDEVICE_TYPE_NONE;
    // AVBufferRef *hwDeviceCtx = nullptr;

    // while ((type = av_hwdevice_iterate_types(type)) != AV_HWDEVICE_TYPE_NONE) {
    //     if (av_hwdevice_ctx_create(&hwDeviceCtx, type, nullptr, nullptr, 0) >= 0) {
    //         // Hardware device found
    //         break;
    //     }
    // }

    // if (hwDeviceCtx) {
    //     // Use the hardware device for decoding
    //     avcCtx->hw_device_ctx = av_buffer_ref(hwDeviceCtx);
    //     avcCtx->get_format = qavhwdevice_get_format;
    //     avcCtx->opaque = new QAVHWDevicePrivate(hwDeviceCtx);
    // } else {
    //     // No hardware device found, use software decoding
    //     avcCtx->hw_device_ctx = nullptr;
    // }
}

void Player::Private::demux()
{
    while (!isTerminate && state != PlayingState) {
        QThread::msleep(100);
    }
    DBG("Starting demuxing");
    avPacket = av_packet_alloc();
    avFrame = av_frame_alloc();


    while (av_read_frame(avfCtx, avPacket) >= 0 && !isTerminate) {
        if (avPacket->stream_index == videoStreamIndex) {
            if (avcodec_send_packet(avcCtx, avPacket) < 0) {
                fprintf(stderr, "Error sending packet for decoding\n");
                break;
            }

            while (avcodec_receive_frame(avcCtx, avFrame) >= 0 && !isTerminate) {
                // Process the frame here (e.g., save it to an image file)
                // ...
                DBG("Frame received avFrameFormat : "+QString::number(avFrame->format));
                // add clone data to enqueue
                AVFrame *frame = av_frame_clone(avFrame);
                frameQueue.enqueue(frame);
                // DBG(QString("Frame received count: %1").arg(frameQueue.count()));
            }
        }

        // av_packet_unref(avPacket);
    }

}

void Player::Private::play()
{
    while (!isTerminate) {
        DBG("Playing");
        AVFrame *frame = frameQueue.dequeue();
        if (frame == nullptr) {
            DBG("Frame is null");
            continue;
        }
        DBG("frame.format : "+QString::number(frame->format));

        // Determine the format of the AVFrame (e.g., AV_PIX_FMT_YUV420P).
        const AVPixelFormat pixelFormat = static_cast<AVPixelFormat>(frame->format);

        // Create a SwsContext for converting pixel format if necessary.
        SwsContext *swsContext = nullptr;

        DBG("pixelFormat: "+QString::number(pixelFormat));
        // Example: Convert AVFrame to QImage (Assuming RGB format).
        if (pixelFormat != AV_PIX_FMT_RGB24) {
            swsContext = sws_getContext(frame->width, frame->height, pixelFormat,
                                        frame->width, frame->height, AV_PIX_FMT_RGB24,
                                        SWS_BILINEAR, nullptr, nullptr, nullptr);
            if (!swsContext) {
                DBG("Error creating SwsContext");
                continue;
            }
        }

        // Allocate memory for the QImage.
        QImage image(frame->width, frame->height, QImage::Format_RGB888);

        // Set the data pointer of the QImage.
        uint8_t *destData[1] = {image.bits()};
        int destLinesize[1] = {static_cast<int>(image.bytesPerLine())};
        if (swsContext) {
            sws_scale(swsContext, frame->data, frame->linesize,
                      0, frame->height, destData, destLinesize);
        } else {
            // If pixel format is already RGB, just copy the data.
            memcpy(destData[0], frame->data[0], frame->linesize[0] * frame->height);
        }

        // Cleanup the SwsContext if created.
        if (swsContext) {
            sws_freeContext(swsContext);
        }

        // Now you can use the 'image' object as needed.
        // For example, display it in a QLabel or save it to a file.
        // Show AVFrame using imshow from OpenCV continously
        DBG("Playing");
        if(videoOutput)
        {
            videoOutput->setRGBParameters(image.width(),image.height());
            videoOutput->setImage(image);
        }
        av_frame_unref(frame);
        av_frame_free(&frame);
        DBG("Playing");
    }
}

Player::Player(QObject *parent)
    : QObject{parent}
    , d{new Private{this}}
{
    av_log_set_level(AV_LOG_ERROR);
}

Player::~Player()
{
    DBG("");
    d->terminate();
    if(d->avPacket)
    {
        av_packet_unref(d->avPacket);
        av_packet_free(&d->avPacket);
    }
    if(d->avFrame)
        av_frame_free(&d->avFrame);

    if(d->avcCtx)
        avcodec_free_context(&d->avcCtx);

    if(d->avfCtx)
        avformat_close_input(&d->avfCtx);

    d.clear();
}

void Player::setSource(const QString &source, QMap<QString, QString> options)
{
    if(d->url == source)
        return;

    d->url = source;
    d->options = options;

    d->terminate();
    d->isTerminate = false;
    d->frameQueue.reset();

    d->loadFuture = QtConcurrent::run(d->threadPool, &Player::Private::load,d);
}

void Player::setVideoOutput(GLWidget *widget)
{
    if(d->videoOutput == widget)
        return;

    d->videoOutput = widget;
}

Player::State Player::state() const
{
    return d->state;
}

//public slots
void Player::play()
{
    d->demuxerFuture = QtConcurrent::run(d->threadPool, &Player::Private::demux,d);
    d->playerFuture = QtConcurrent::run(d->threadPool, &Player::Private::play,d);
}

void Player::pause()
{

}

void Player::stop()
{

}
