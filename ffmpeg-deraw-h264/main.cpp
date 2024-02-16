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

    // Open input file (replace "input.mp4" with your input file)
    AVFormatContext* format_ctx = avformat_alloc_context();
    if (avformat_open_input(&format_ctx, "D:\\Codes\\NGS_documents\\video_sample.264", nullptr, nullptr) != 0) {
        // Error handling
        return -1;
    }

    // Retrieve stream information
    if (avformat_find_stream_info(format_ctx, nullptr) < 0) {
        // Error handling
        avformat_close_input(&format_ctx);
        return -1;
    }

    // Find the video stream
    int video_stream_index = -1;
    for (unsigned int i = 0; i < format_ctx->nb_streams; i++) {
        if (format_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_stream_index = i;
            break;
        }
    }

    if (video_stream_index == -1) {
        // No video stream found
        avformat_close_input(&format_ctx);
        return -1;
    }

    // Find and open the video decoder
    const AVCodec* codec = avcodec_find_decoder(format_ctx->streams[video_stream_index]->codecpar->codec_id);
    if (!codec) {
        // Codec not found
        avformat_close_input(&format_ctx);
        return -1;
    }

    AVCodecContext* codec_ctx = avcodec_alloc_context3(codec);
    if (!codec_ctx) {
        // Error allocating codec context
        avformat_close_input(&format_ctx);
        return -1;
    }

    if (avcodec_open2(codec_ctx, codec, nullptr) < 0) {
        // Error opening codec
        avcodec_free_context(&codec_ctx);
        avformat_close_input(&format_ctx);
        return -1;
    }

    // Allocate a frame for decoded output
    AVFrame* frame = av_frame_alloc();
    if (!frame) {
        // Error allocating frame
        avcodec_free_context(&codec_ctx);
        avformat_close_input(&format_ctx);
        return -1;
    }

    // Read and decode frames
    AVPacket packet;
    int frameCount = 0;
    while (av_read_frame(format_ctx, &packet) >= 0) {
        if (packet.stream_index == video_stream_index) {
            if (avcodec_send_packet(codec_ctx, &packet) == 0 &&
                avcodec_receive_frame(codec_ctx, frame) == 0) {
                // Successfully decoded a frame, do something with it
                // For example, process or display the frame
                // Check if any linesize is zero
                for (int i = 0; i < AV_NUM_DATA_POINTERS; ++i) {
                    if (frame->linesize[i] <= 0) {
                        qDebug() << "Incomplete frame data at plane" << i;
                        break;
                    }
                    if(i==AV_NUM_DATA_POINTERS-1){
                        QImage image = convertAVFrameToQImage(frame);
                        qDebug()<<"afterconvert";
                        image.save("image_"+QString::number(frameCount++)+".png");
                    }
                }
            }
        }
        av_packet_unref(&packet);
    }

    // Free resources
    av_frame_free(&frame);
    avcodec_free_context(&codec_ctx);
    avformat_close_input(&format_ctx);

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
