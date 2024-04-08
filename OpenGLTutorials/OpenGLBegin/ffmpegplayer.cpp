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
#include <QImage>


FfmpegPlayer::FfmpegPlayer(FrameQueue *queue, QObject *parent)
    :
    QThread{parent},
    queue(queue)
{
    m_url ="";
}

// QImage avFrameToQImage(const AVFrame *frame) {
//     if (!frame)
//         return QImage();

//     /* 1. Get frame and QImage to show */
//     QImage *myImage = new QImage(1920, 1080, QImage::Format_RGBA8888);

//     /* 2. Convert and write into image buffer  */
//     uint8_t *dst[] = {myImage->bits()};
//     int linesizes[4];
//     av_image_fill_linesizes(linesizes, AV_PIX_FMT_RGBA, frame->width);

//     sws_scale(myswscontext, frame->data, (const int*)frame->linesize,
//               0, frame->height, dst, linesizes);

//     // Free the SwsContext
//     sws_freeContext(swsCtx);

//     // Delete the RGB data buffer
//     delete[] rgbData;

//     return image;
// }

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

// Convert BGR24 to RGB24
void convertBGR24toRGB24(const char* bgr24Data, char* rgb24Data, int width, int height) {
    for (int i = 0; i < width * height ; ++i) {
        rgb24Data[3 * i + 0] = bgr24Data[3 * i + 2]; // Red
        rgb24Data[3 * i + 1] = bgr24Data[3 * i + 1]; // Green
        rgb24Data[3 * i + 2] = bgr24Data[3 * i + 0]; // Blue
    }
}

// Convert RGB24 to YUV420
void convertRGB24toYUV420(const char* rgb24Data, char* yuv420Data, int width, int height) {
    int imageSize = width * height;
    int uvSize = (width / 2) * (height / 2);

    // Y plane
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int index = i * width + j;
            int y = (int)(0.299 * rgb24Data[3 * index] + 0.587 * rgb24Data[3 * index + 1] + 0.114 * rgb24Data[3 * index + 2]);
            yuv420Data[index] = (uint8_t)y;
        }
    }

    // U and V planes
    for (int i = 0; i < height / 2; ++i) {
        for (int j = 0; j < width / 2; ++j) {
            int index = i * (width / 2) + j;
            int uvIndex = imageSize + i * (width / 2) + j;
            int u = (int)(-0.147 * rgb24Data[6 * index] - 0.289 * rgb24Data[6 * index + 1] + 0.436 * rgb24Data[6 * index + 2]
                           + 128);
            int v = (int)(0.615 * rgb24Data[6 * index] - 0.515 * rgb24Data[6 * index + 1] - 0.100 * rgb24Data[6 * index + 2]
                           + 128);
            yuv420Data[imageSize + uvIndex] = (uint8_t)u;
            yuv420Data[imageSize + uvSize + uvIndex] = (uint8_t)v;
        }
    }
}

void FfmpegPlayer::run()
{
    m_isRunning = true;
    // realDecoded();
    fakeDecoded();
}

void FfmpegPlayer::stop()
{
    m_isRunning = false;
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
                if(queue != NULL){
                    YUVFrame frame = YUVFrame{
                        QByteArray(yuvData),
                        width,
                        height
                    };
                    frame.stride = stride;
                    queue->pushFrame(frame);
                }
            }
        }
        av_packet_unref(&packet);

        if(!m_isRunning){
            break;
        }
    }

    av_frame_free(&frame);
    avcodec_free_context(&codecContext);
    avformat_close_input(&formatContext);
    avformat_free_context(formatContext);
}

enum ImageFormat {
    Unknown,
    RGB,
    BGR,
    ARGB,
    BGRA,
    RGBA,
    ABGR,
    GRAY // Grayscale
};

ImageFormat detectImageFormat(const char* imageData) {
    if (imageData[0] == 'R' && imageData[1] == 'G' && imageData[2] == 'B')
        return RGB;
    else if (imageData[0] == 'B' && imageData[1] == 'G' && imageData[2] == 'R')
        return BGR;
    else if (imageData[0] == 'A' && imageData[1] == 'R' && imageData[2] == 'G' && imageData[3] == 'B')
        return ARGB;
    else if (imageData[0] == 'B' && imageData[1] == 'G' && imageData[2] == 'R' && imageData[3] == 'A')
        return BGRA;
    else if (imageData[0] == 'R' && imageData[1] == 'G' && imageData[2] == 'B' && imageData[3] == 'A')
        return RGBA;
    else if (imageData[0] == 'A' && imageData[1] == 'B' && imageData[2] == 'G' && imageData[3] == 'R')
        return ABGR;
    else if (imageData[0] == 'G' && imageData[1] == 'R' && imageData[2] == 'A' && imageData[3] == 0)
        return GRAY; // Grayscale (alpha channel indicates transparency)

    return Unknown;
}

void FfmpegPlayer::fakeDecoded()
{
    const int w = 1920, h = 1080;
    while (m_isRunning) {
        QThread::msleep(100);
        QString imgPath = QString(":/images/file_0.txt")/*.arg(rand()%5)*/;
        QFile f(imgPath);
        f.open(QIODevice::ReadOnly);
        QByteArray data(f.readAll());
        QImage img = QImage(imgPath,"BGR");
        qDebug()<<img.format();
        // Allocate memory for RGB24 and YUV420 data
        qDebug("data size: %d, width*height*3 : %d, format %d", data.size(),w*h*3,detectImageFormat(data.constData()));
        char* rgb24Data = new char[w * h * 3]; // RGB24 data
        char* yuv420Data = new char[w * h * 3 / 2]; // YUV420 data

        // Convert BGR24 to RGB24
        convertBGR24toRGB24(data.data(), rgb24Data, w, h);

        // Convert RGB24 to YUV420
        convertRGB24toYUV420(rgb24Data, yuv420Data, w, h);



        if(queue != NULL){
            queue->pushFrame({
                yuv420Data,w,h,NULL
            });
        }
    }
}


