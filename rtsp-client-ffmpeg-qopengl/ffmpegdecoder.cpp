extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

#include "ffmpegdecoder.h"
#include <thread>
#include <QDebug>

FFmpegDecoder::FFmpegDecoder(QObject *parent)
    : QObject(parent)
{
}

void FFmpegDecoder::start(const QString &url)
{
    const std::string s = url.toStdString();

    std::thread([this, s]() {
        avformat_network_init();

        AVFormatContext *pFormatContext = avformat_alloc_context();

        if (avformat_open_input(&pFormatContext, s.c_str(), nullptr, nullptr) != 0)
        {
            qWarning() << "Error opening RTSP stream";
            emit finished();
            return;
        }

        if (avformat_find_stream_info(pFormatContext, nullptr) < 0)
        {
            qWarning() << "Error finding stream info";
            avformat_close_input(&pFormatContext);
            emit finished();
            return;
        }

        int videoStreamIndex = -1;
        for (unsigned int i = 0; i < pFormatContext->nb_streams; i++)
        {
            if (pFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
            {
                videoStreamIndex = i;
                break;
            }
        }

        if (videoStreamIndex == -1)
        {
            qWarning() << "No video stream found";
            avformat_close_input(&pFormatContext);
            emit finished();
            return;
        }

        AVCodecContext *pCodecContext = avcodec_alloc_context3(nullptr);
        avcodec_parameters_to_context(pCodecContext, pFormatContext->streams[videoStreamIndex]->codecpar);
        const AVCodec *pCodec = avcodec_find_decoder(pCodecContext->codec_id);

        if (!pCodec)
        {
            qWarning() << "Codec not found";
            avcodec_free_context(&pCodecContext);
            avformat_close_input(&pFormatContext);
            emit finished();
            return;
        }

        if (avcodec_open2(pCodecContext, pCodec, nullptr) < 0)
        {
            qWarning() << "Failed to open codec";
            avcodec_free_context(&pCodecContext);
            avformat_close_input(&pFormatContext);
            emit finished();
            return;
        }

        AVPacket *pPacket = av_packet_alloc();
        AVFrame *pFrame = av_frame_alloc();

        SwsContext *swsCtx = nullptr;

        while (av_read_frame(pFormatContext, pPacket) >= 0)
        {
            if (pPacket->stream_index == videoStreamIndex)
            {
                if (avcodec_send_packet(pCodecContext, pPacket) < 0)
                {
                    qWarning() << "Error sending packet";
                    break;
                }

                while (avcodec_receive_frame(pCodecContext, pFrame) >= 0)
                {
                    const AVPixelFormat pixFmt = static_cast<AVPixelFormat>(pFrame->format);

                    if (!swsCtx)
                    {
                        swsCtx = sws_getContext(pFrame->width, pFrame->height, pixFmt,
                                                pFrame->width, pFrame->height, AV_PIX_FMT_RGB24,
                                                SWS_BILINEAR, nullptr, nullptr, nullptr);
                        if (!swsCtx)
                        {
                            qWarning() << "Failed to create SwsContext";
                            break;
                        }
                    }

                    QImage img(pFrame->width, pFrame->height, QImage::Format_RGB888);
                    uint8_t *dest[1] = { img.bits() };
                    int destLines[1] = { static_cast<int>(img.bytesPerLine()) };

                    sws_scale(swsCtx, pFrame->data, pFrame->linesize, 0, pFrame->height, dest, destLines);

                    emit frameReady(img);

                    av_frame_unref(pFrame);
                }
            }

            av_packet_unref(pPacket);
        }

        if (swsCtx)
            sws_freeContext(swsCtx);

        av_packet_free(&pPacket);
        av_frame_free(&pFrame);
        avcodec_free_context(&pCodecContext);
        avformat_close_input(&pFormatContext);

        emit finished();
    }).detach();
}
