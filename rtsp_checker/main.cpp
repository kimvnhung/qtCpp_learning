extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

#include <QDebug>
#include <QDateTime>

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

    bool readFrame = false;
    while (av_read_frame(pFormatContext, pPacket) >= 0 && !readFrame) {
        if (pPacket->stream_index == videoStreamIndex) {
            if (avcodec_send_packet(pCodecContext, pPacket) < 0) {
                fprintf(stderr, "Error sending packet for decoding\n");
                break;
            }

            while (avcodec_receive_frame(pCodecContext, pFrame) >= 0) {
                // Process the frame here (e.g., save it to an image file)
                // ...
                // qDebug()<<"Frame received";
                av_frame_unref(pFrame);
                readFrame = true;
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
