extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;

#include <QImage>
#include <QDebug>
#include <QDateTime>

// Assume AVFrame* frame is already initialized and contains video frame data.

void processFrame(AVFrame *frame) {
    // Determine the format of the AVFrame (e.g., AV_PIX_FMT_YUV420P).
    const AVPixelFormat pixelFormat = static_cast<AVPixelFormat>(frame->format);

    qDebug()<<"pixelFormat: "<<pixelFormat;
    // Create a SwsContext for converting pixel format if necessary.
    SwsContext *swsContext = nullptr;

    // Example: Convert AVFrame to QImage (Assuming RGB format).
    if (pixelFormat != AV_PIX_FMT_RGB24) {
        swsContext = sws_getContext(frame->width, frame->height, pixelFormat,
                                    frame->width, frame->height, AV_PIX_FMT_RGB24,
                                    SWS_BILINEAR, nullptr, nullptr, nullptr);
        if (!swsContext) {
            qDebug() << "Error creating SwsContext";
            return;
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


    image.save("image_"+QString::number(QDateTime::currentMSecsSinceEpoch())+".png");

    // Cleanup the SwsContext if created.
    if (swsContext) {
        sws_freeContext(swsContext);
    }

    // Now you can use the 'image' object as needed.
    // For example, display it in a QLabel or save it to a file.
    // Show AVFrame using imshow from OpenCV continously
    static bool showed = false;
    if(!showed){
        // clone data from frame to new Mat
        Mat img(frame->height, frame->width, CV_8UC3, image.bits(), image.bytesPerLine());
        imshow("Frame", img);
        showed = true;
    }

}

int main() {
    // av_register_all();

    AVFormatContext *pFormatContext = avformat_alloc_context();
    if (avformat_open_input(&pFormatContext, "rtsp://admin:abcd1234@192.168.1.250:554/1/1?transmode=unicast&profile=vaom", nullptr, nullptr) != 0) {
        fprintf(stderr, "Error opening RTSP stream\n");
        return -1;
    }

    if (avformat_find_stream_info(pFormatContext, nullptr) < 0) {
        fprintf(stderr, "Error finding stream information\n");
        return -1;
    }

    int videoStreamIndex = -1;
    for (unsigned int i = 0; i < pFormatContext->nb_streams; i++) {
        if (pFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex = i;
            break;
        }
    }

    if (videoStreamIndex == -1) {
        fprintf(stderr, "Error finding video stream\n");
        return -1;
    }

    AVCodecContext *pCodecContext = avcodec_alloc_context3(nullptr);
    avcodec_parameters_to_context(pCodecContext, pFormatContext->streams[videoStreamIndex]->codecpar);
    const AVCodec *pCodec = avcodec_find_decoder(pCodecContext->codec_id);

    if (pCodec == nullptr) {
        fprintf(stderr, "Error finding codec\n");
        return -1;
    }

    if (avcodec_open2(pCodecContext, pCodec, nullptr) < 0) {
        fprintf(stderr, "Error opening codec\n");
        return -1;
    }

    AVPacket *pPacket = av_packet_alloc();
    AVFrame *pFrame = av_frame_alloc();

    while (av_read_frame(pFormatContext, pPacket) >= 0) {
        if (pPacket->stream_index == videoStreamIndex) {
            if (avcodec_send_packet(pCodecContext, pPacket) < 0) {
                fprintf(stderr, "Error sending packet for decoding\n");
                break;
            }

            while (avcodec_receive_frame(pCodecContext, pFrame) >= 0) {
                // Process the frame here (e.g., save it to an image file)
                // ...
                // qDebug()<<"Frame received";
                processFrame(pFrame);
                av_frame_unref(pFrame);
            }
        }

        av_packet_unref(pPacket);
    }

    av_packet_free(&pPacket);
    av_frame_free(&pFrame);
    avcodec_free_context(&pCodecContext);
    avformat_close_input(&pFormatContext);

    return 0;
}
