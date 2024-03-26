#include "ffmpegplayer.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/pixfmt.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

#include <QDebug>
#include <QFile>


FfmpegPlayer::FfmpegPlayer(FrameQueue *queue, QObject *parent)
    :
    QThread{parent},
    queue(queue)
{
    m_url ="";
}

void convertFrameToYUV(const AVFrame *frame, char **yuvData, int *width, int *height, int *stride) {
    // Extract width, height, and stride
    *width = frame->width;
    *height = frame->height;
    stride[0] = frame->linesize[0]; // Y stride
    stride[1] = frame->linesize[1]; // U stride
    stride[2] = frame->linesize[2]; // V stride

    // Allocate memory for the YUV data
    int totalSize = *width * *height + 2 * ((*width + 1) / 2) * ((*height + 1) / 2) * 2; // Y + UV planes
    *yuvData = new char[totalSize];

    // Copy Y, U, V data to the contiguous buffer
    char *ptr = *yuvData;
    for (int i = 0; i < *height; i++) {
        memcpy(ptr, frame->data[0] + i * stride[0], *width);
        ptr += *width;
    }
    for (int i = 0; i < *height / 2; i++) {
        memcpy(ptr, frame->data[1] + i * stride[1], *width / 2);
        ptr += *width / 2;
    }
    for (int i = 0; i < *height / 2; i++) {
        memcpy(ptr, frame->data[2] + i * stride[2], *width / 2);
        ptr += *width / 2;
    }
}

void FfmpegPlayer::run()
{
    fakeDecoded();
}

void FfmpegPlayer::realDecoded()
{
    // av_register_all();

    AVFormatContext *formatContext = avformat_alloc_context();
    if (!formatContext) {
        fprintf(stderr, "Could not allocate format context\n");
        return;
    }

    if (avformat_open_input(&formatContext, "rtsp://admin:abcd1234@14.241.65.40:554/cam/realmonitor?channel=1&subtype=0&unicast=true&proto=Onvif", NULL, NULL) != 0) {
        fprintf(stderr, "Failed to open RTSP stream\n");
        return;
    }

    if (avformat_find_stream_info(formatContext, NULL) < 0) {
        fprintf(stderr, "Failed to find stream info\n");
        return;
    }

    const AVCodec *codec = NULL;
    int videoStreamIndex = -1;

    for (int i = 0; i < formatContext->nb_streams; i++) {
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex = i;
            codec = avcodec_find_decoder(formatContext->streams[i]->codecpar->codec_id);
            if (!codec) {
                fprintf(stderr, "Failed to find decoder\n");
                return;
            }
            break;
        }
    }

    if (videoStreamIndex == -1) {
        fprintf(stderr, "Could not find video stream in the input\n");
        return;
    }

    AVCodecContext *codecContext = avcodec_alloc_context3(codec);
    if (!codecContext) {
        fprintf(stderr, "Failed to allocate codec context\n");
        return;
    }

    if (avcodec_parameters_to_context(codecContext, formatContext->streams[videoStreamIndex]->codecpar) < 0) {
        fprintf(stderr, "Failed to copy codec parameters to codec context\n");
        return;
    }

    if (avcodec_open2(codecContext, codec, NULL) < 0) {
        fprintf(stderr, "Failed to open codec\n");
        return;
    }

    AVFrame *frame = av_frame_alloc();
    if (!frame) {
        fprintf(stderr, "Failed to allocate frame\n");
        return;
    }

    AVPacket packet;
    av_init_packet(&packet);

    while (av_read_frame(formatContext, &packet) >= 0) {
        if (packet.stream_index == videoStreamIndex) {
            int response = avcodec_send_packet(codecContext, &packet);
            if (response < 0) {
                qDebug()<<"Error while sending a packet to the decoder: ";
                break;
            }
            while (response >= 0) {
                response = avcodec_receive_frame(codecContext, frame);
                if (response == AVERROR(EAGAIN) || response == AVERROR_EOF)
                    break;
                else if (response < 0) {
                    qDebug()<<"Error while receiving a frame from the decoder: ";
                    return;
                }

                // Do something with the frame (e.g., display it)
                qDebug()<<"display";
                char *yuvData;
                int width, height;
                int stride[3]; // 0: Y stride, 1: U stride, 2: V stride
                convertFrameToYUV(frame,&yuvData,&width,&height,stride);
                // emit frameAvailable(yuvData,width,height,stride);
            }
        }
        av_packet_unref(&packet);
    }

    av_frame_free(&frame);
    avcodec_free_context(&codecContext);
    avformat_close_input(&formatContext);
    avformat_free_context(formatContext);
}

void FfmpegPlayer::fakeDecoded()
{
    while (true) {
        QThread::msleep(100);
        QString imgPath;
        if(rand()%2==0){
            imgPath = ":/images/1280x720.yuv";
        }else {
            imgPath = ":/images/1280x720_2.yuv";
        }
        QFile f(imgPath);
        f.open(QIODevice::ReadOnly);
        QByteArray data(f.readAll());
        qDebug("data size: %d", data.size());
        const int w = 1280, h = 720;
        if(queue != NULL){
            queue->pushFrame({
                data,w,h,NULL
            });
        }
    }
}


