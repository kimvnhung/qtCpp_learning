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
#include <libavutil/time.h>
}

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <QFuture>
#include <QtConcurrent>
#include <QDateTime>

#define BUFFER_SIZE 32

class Player::Private
{
public:
    Private(Player *owner)
        :owner{owner}
        , avfCtx{nullptr}
        , avcCtx{nullptr}
        , avCodec{nullptr}
        , avStream{nullptr}
        , avPacket{nullptr}
        , avFrame{nullptr}
        , videoStreamIndex{-1}
        , frameQueue{AsyncQueue<AVFrame*>(owner, BUFFER_SIZE)}
        , videoOutput{nullptr}
        , url{""}
        , options{}
        , threadPool{new QThreadPool(owner)}
        , ptsCounter{0}
        , isTerminate{false}
        , state{StoppedState}
    {
        threadPool->setMaxThreadCount(5);
    }

    Player *owner;

    AVFormatContext *avfCtx;
    AVCodecContext *avcCtx;
    const AVCodec *avCodec;
    AVStream *avStream;
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
    QFuture<void> consumerFuture;
    QFuture<void> clockFuture;

    void load();
    void countClock();
    void setUpClock();
    qint64 ptsCounter;
    QMutex ptsMutex;

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
    DBG("");
    frameQueue.terminate();
    DBG("");
    loadFuture.waitForFinished();
    DBG("");
    clockFuture.waitForFinished();
    DBG("");
    demuxerFuture.waitForFinished();
    DBG("");
    consumerFuture.waitForFinished();
    DBG("");
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

    avcCtx = avcodec_alloc_context3(nullptr);

    videoStreamIndex = av_find_best_stream(avfCtx, AVMEDIA_TYPE_VIDEO, -1, -1, &avCodec, 0);

    if (videoStreamIndex == -1) {
        fprintf(stderr, "Error finding video stream\n");
        setState(State::StoppedState, {ResourceError, "Error finding video stream"});
    }

    avStream = avfCtx->streams[videoStreamIndex];
    detectHardwareDevice();

    avcodec_parameters_to_context(avcCtx, avStream->codecpar);
    avCodec = avcodec_find_decoder(avcCtx->codec_id);

    if (avCodec == nullptr) {
        fprintf(stderr, "Error finding codec\n");
        setState(State::StoppedState, {ResourceError, "Error finding codec"});
    }


    if (avcodec_open2(avcCtx, avCodec, NULL) < 0) {
        fprintf(stderr, "Error opening codec\n");
        setState(State::StoppedState, {ResourceError, "Error opening codec"});
    }

    setState(State::PlayingState);
}

void Player::Private::countClock()
{
    // Block until avStream is not NULL or return if isTerminate is true
    while (!isTerminate && avStream == nullptr) {
        QThread::sleep(std::chrono::milliseconds{100});
    }
    // Convert AVStream time_base to nanoseconds
    int timeBase = av_q2d(avStream->time_base)*1000000*1000;


    QElapsedTimer timer;
    qint64 startTime = 0;
    while (!isTerminate) {
        if (state != PlayingState) {
            QThread::sleep(std::chrono::milliseconds{100});
            continue;
        }

        if(ptsCounter==0)
            startTime = timer.nsecsElapsed();

        {
            QMutexLocker locker(&ptsMutex);
            ptsCounter = (timer.nsecsElapsed()-startTime)/(timeBase);
        }
        //Sleep for the calculated duration
        QThread::sleep(std::chrono::milliseconds{10});
    }
}

static enum AVPixelFormat get_hw_pix_fmt(AVCodecContext *ctx, const enum AVPixelFormat *pix_fmts) {
    while (*pix_fmts != AV_PIX_FMT_NONE) {
        if (*pix_fmts == AV_PIX_FMT_CUDA || *pix_fmts == AV_PIX_FMT_VAAPI || *pix_fmts == AV_PIX_FMT_DXVA2_VLD) {
            return *pix_fmts;
        }
        pix_fmts++;
    }
    fprintf(stderr, "Failed to get HW surface format.\n");
    return AV_PIX_FMT_NONE;
}

int init_hw_decoder(AVCodecContext *ctx, enum AVHWDeviceType type) {
    int err;
    AVBufferRef *hw_device_ctx = NULL;

    if ((err = av_hwdevice_ctx_create(&hw_device_ctx, type, NULL, NULL, 0)) < 0) {
        fprintf(stderr, "Failed to create specified HW device.\n");
        return err;
    }
    ctx->hw_device_ctx = av_buffer_ref(hw_device_ctx);
    av_buffer_unref(&hw_device_ctx);
    return err;
}

void Player::Private::detectHardwareDevice()
{
    // Detect the hardware type
    // enum AVHWDeviceType hw_type = AV_HWDEVICE_TYPE_NONE;
    // const char *hw_type_name = NULL;
    // while ((hw_type = av_hwdevice_iterate_types(hw_type)) != AV_HWDEVICE_TYPE_NONE) {
    //     hw_type_name = av_hwdevice_get_type_name(hw_type);
    //     printf("Found hardware type: %s\n", hw_type_name);

    //     // Break if the hardware type is suitable for current platform
    //     if (hw_type == AV_HWDEVICE_TYPE_CUDA
    //         || hw_type == AV_HWDEVICE_TYPE_VAAPI
    //         || hw_type == AV_HWDEVICE_TYPE_DXVA2
    //         || hw_type == AV_HWDEVICE_TYPE_D3D11VA
    //         || hw_type == AV_HWDEVICE_TYPE_QSV
    //         || hw_type == AV_HWDEVICE_TYPE_OPENCL
    //         || hw_type == AV_HWDEVICE_TYPE_VULKAN) {
    //         printf("Use hardware type: %s\n", hw_type_name);
    //         break;
    //     }
    // }

    // if (hw_type == AV_HWDEVICE_TYPE_NONE) {
    //     fprintf(stderr, "No supported hardware device found.\n");
    //     return;
    // }

    // // Initialize hardware decoder
    // if (init_hw_decoder(avcCtx, hw_type) < 0) {
    //     fprintf(stderr, "Failed to initialize hardware decoder.\n");
    //     return;
    // }

    // avcCtx->get_format = get_hw_pix_fmt;
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
                // DBG("Frame received avFrameFormat : "+QString::number(avFrame->format));
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
    int lastWidth = 0, lastHeight = 0;
    while (!isTerminate) {
        AVFrame *frame = frameQueue.front();
        if (frame == nullptr) {
            DBG("Frame is null");
            continue;
        }

        {
            QMutexLocker locker(&ptsMutex);
            if(frame->pts == AV_NOPTS_VALUE)
            {
                DBG("Frame pts is not valid");
            }
            else if(frame->pts > ptsCounter)
            {
                // DBG(QString("Frame pts : %1, ptsCounter : %2").arg(frame->pts).arg(ptsCounter));
                continue;
            }else {
                // DBG(QString("Frame pts : %1, tempCounter : %2").arg(frame->pts).arg(ptsCounter));
                //Calculate the pts in the second
                // qint64 pts = frame->pts * av_q2d(avStream->time_base);
                // DBG(QString("Frame pts : %1").arg(pts));
                frameQueue.pop();
            }
        }

        Q_EMIT owner->positionChanged(frame->pts*av_q2d(avStream->time_base)*1000);


        // Determine the format of the AVFrame (e.g., AV_PIX_FMT_YUV420P).
        const AVPixelFormat pixelFormat = static_cast<AVPixelFormat>(frame->format);

        // Create a SwsContext for converting pixel format if necessary.
        SwsContext *swsContext = nullptr;

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
        if(videoOutput)
        {
            if(image.width() != lastWidth || image.height() != lastHeight)
            {
                lastWidth = image.width();
                lastHeight = image.height();
                videoOutput->setRGBParameters(image.width(),image.height());
            }
            videoOutput->setImage(image);
        }
        av_frame_unref(frame);
        av_frame_free(&frame);
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
    DBG("");
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
    d->consumerFuture = QtConcurrent::run(d->threadPool, &Player::Private::play,d);
    d->clockFuture = QtConcurrent::run(d->threadPool, &Player::Private::countClock,d);

}

void Player::pause()
{

}

void Player::stop()
{

}
