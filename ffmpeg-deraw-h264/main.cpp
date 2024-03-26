#include "mainwindow.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/pixfmt.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

#include <QApplication>

int decodeH264();
QImage convertAVFrameToQImage(const AVFrame* frame);
void convertYUV420pToRGB(const unsigned char* yuvData, int width, int height, unsigned char* rgbData);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    qDebug()<<"decodeResult"<<decodeH264();
    return a.exec();
}

int decodeH264()
{
    // av_register_all();

    AVFormatContext *formatContext = avformat_alloc_context();
    if (!formatContext) {
        fprintf(stderr, "Could not allocate format context\n");
        return 1;
    }

    if (avformat_open_input(&formatContext, "rtsp://admin:abcd1234@14.241.65.40:554/cam/realmonitor?channel=1&subtype=0&unicast=true&proto=Onvif", NULL, NULL) != 0) {
        fprintf(stderr, "Failed to open RTSP stream\n");
        return 1;
    }

    if (avformat_find_stream_info(formatContext, NULL) < 0) {
        fprintf(stderr, "Failed to find stream info\n");
        return 1;
    }

    const AVCodec *codec = NULL;
    int videoStreamIndex = -1;

    for (int i = 0; i < formatContext->nb_streams; i++) {
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex = i;
            codec = avcodec_find_decoder(formatContext->streams[i]->codecpar->codec_id);
            if (!codec) {
                fprintf(stderr, "Failed to find decoder\n");
                return 1;
            }
            break;
        }
    }

    if (videoStreamIndex == -1) {
        fprintf(stderr, "Could not find video stream in the input\n");
        return 1;
    }

    AVCodecContext *codecContext = avcodec_alloc_context3(codec);
    if (!codecContext) {
        fprintf(stderr, "Failed to allocate codec context\n");
        return 1;
    }

    if (avcodec_parameters_to_context(codecContext, formatContext->streams[videoStreamIndex]->codecpar) < 0) {
        fprintf(stderr, "Failed to copy codec parameters to codec context\n");
        return 1;
    }

    if (avcodec_open2(codecContext, codec, NULL) < 0) {
        fprintf(stderr, "Failed to open codec\n");
        return 1;
    }

    AVFrame *frame = av_frame_alloc();
    if (!frame) {
        fprintf(stderr, "Failed to allocate frame\n");
        return 1;
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
                    return 1;
                }

                // Do something with the frame (e.g., display it)
                qDebug()<<"display";
            }
        }
        av_packet_unref(&packet);
    }

    av_frame_free(&frame);
    avcodec_free_context(&codecContext);
    avformat_close_input(&formatContext);
    avformat_free_context(formatContext);

    return 0;
}

// Assume AVFrame* frame is your input AVFrame

QImage convertAVFrameToQImage(const AVFrame* frame) {
    if (!frame) {
        qDebug() << "Invalid AVFrame";
        return QImage();
    }

    // Determine the format of the AVFrame
    AVPixelFormat frameFormat = static_cast<AVPixelFormat>(frame->format);

    // Check if the frame is in YUV420P format
    if (frameFormat == AV_PIX_FMT_YUV420P) {
        unsigned char* rgbData = (unsigned char*)malloc(frame->width * frame->height * 3);
        convertYUV420pToRGB((unsigned char*)frame->data,frame->width,frame->height,rgbData);
        // QImage image(rgbData,frame->width,frame->height,QImage::Format::Format_RGB888);
        // free(rgbData);
        // return image;
    }

    // Handle other pixel formats (e.g., RGB)
    // Implement conversion for other formats if needed

    qDebug() << "Unsupported pixel format:" << frameFormat;
    return QImage();
}

void convertYUV420pToRGB(const unsigned char* yuvData, int width, int height, unsigned char* rgbData) {
    int size = width * height;
    int uvSize = size / 4;

    const unsigned char* y = yuvData;
    const unsigned char* u = yuvData + size;
    const unsigned char* v = yuvData + size + uvSize;

    qDebug()<<"width:"<<width<<",height:"<<height;
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            qDebug()<<"i:"<<i<<",j:"<<j;
            qDebug()<<"size : "<<size<<",size+uvSize :"<<(size+uvSize);
            qDebug()<<"Yidx:"<<(j * width + i)<<",Uidx : "<<((j / 2) * (width / 2) + (i / 2))<<",Videx:"<<((j / 2) * (width / 2) + (i / 2));
            int Y = y[j * width + i];
            int U = u[(j / 2) * (width / 2) + (i / 2)];
            int V = v[(j / 2) * (width / 2) + (i / 2)];

            // Convert YUV to RGB
            // int C = Y - 16;
            // int D = U - 128;
            // int E = V - 128;

            // int R = (298 * C + 409 * E + 128) >> 8;
            // int G = (298 * C - 100 * D - 208 * E + 128) >> 8;
            // int B = (298 * C + 516 * D + 128) >> 8;

            // // Clamp values to [0, 255]
            // R = std::max(0, std::min(255, R));
            // G = std::max(0, std::min(255, G));
            // B = std::max(0, std::min(255, B));

            // // Store RGB values
            // rgbData[(j * width + i) * 3] = R;
            // rgbData[(j * width + i) * 3 + 1] = G;
            // rgbData[(j * width + i) * 3 + 2] = B;
        }
    }
}
