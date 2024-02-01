#include <QCoreApplication>

#include <iostream>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <libavformat/avformat.h>
#include <libavutil/mathematics.h>
#include <libavutil/time.h>
}

int main(int argc, char *argv[])
{
    // av_register_all();
    avformat_network_init();

    // Input video file path
    const char* input_filename = "D:\\Codes\\NGS_documents\\videoplayback.mp4";

    // RTSP server address and port
    const char* rtsp_address = "127.0.0.1";
    const int rtsp_port = 8542;

    AVFormatContext* format_ctx = avformat_alloc_context();
    if (!format_ctx) {
        av_log(nullptr, AV_LOG_ERROR, "Cannot allocate AVFormatContext\n");
        return -1;
    }

    // Open input file
    if (avformat_open_input(&format_ctx, input_filename, nullptr, nullptr) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Cannot open input file\n");
        return -1;
    }

    // Retrieve stream information
    if (avformat_find_stream_info(format_ctx, nullptr) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Cannot find stream information\n");
        avformat_close_input(&format_ctx);
        return -1;
    }

    // Create RTSP context
    const AVOutputFormat* rtsp_format = av_guess_format("rtsp", nullptr, nullptr);
    if (!rtsp_format) {
        av_log(nullptr, AV_LOG_ERROR, "Cannot find RTSP output format\n");
        avformat_close_input(&format_ctx);
        return -1;
    }

    AVFormatContext* rtsp_ctx = avformat_alloc_context();
    if (!rtsp_ctx) {
        av_log(nullptr, AV_LOG_ERROR, "Cannot allocate RTSP AVFormatContext\n");
        avformat_close_input(&format_ctx);
        return -1;
    }

    rtsp_ctx->oformat = rtsp_format;

    // Set RTSP server options
    AVDictionary* rtsp_options = nullptr;
    av_dict_set(&rtsp_options, "rtsp_flags", "listen", 0);
    av_dict_set(&rtsp_options, "fflags", "nobuffer", 0); // Optional, to reduce latency

    // Set the RTSP server address and port
    char rtsp_url[100];
    snprintf(rtsp_url, sizeof(rtsp_url), "rtsp://%s:%d/live", rtsp_address, rtsp_port);

    if (avformat_alloc_output_context2(&rtsp_ctx, nullptr, "rtsp", rtsp_url) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Cannot allocate RTSP output context\n");
        avformat_close_input(&format_ctx);
        avformat_free_context(rtsp_ctx);
        return -1;
    }

    // Add video stream to RTSP context
    AVStream* video_stream = avformat_new_stream(rtsp_ctx, nullptr);
    if (!video_stream) {
        av_log(nullptr, AV_LOG_ERROR, "Cannot create video stream in RTSP context\n");
        avformat_close_input(&format_ctx);
        avformat_free_context(rtsp_ctx);
        return -1;
    }

    // Set the AVCodecParameters for the video stream
    if (avcodec_parameters_copy(video_stream->codecpar, format_ctx->streams[0]->codecpar) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Cannot copy codec parameters to RTSP context\n");
        avformat_close_input(&format_ctx);
        avformat_free_context(rtsp_ctx);
        return -1;
    }

    // Open the RTSP server
    if (avio_open(&rtsp_ctx->pb, rtsp_url, AVIO_FLAG_WRITE) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Cannot open RTSP server\n");
        avformat_close_input(&format_ctx);
        avformat_free_context(rtsp_ctx);
        return -1;
    }

    // Write the RTSP context header
    if (avformat_write_header(rtsp_ctx, &rtsp_options) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Cannot write RTSP header\n");
        avio_close(rtsp_ctx->pb);
        avformat_free_context(rtsp_ctx);
        avformat_close_input(&format_ctx);
        return -1;
    }

    // Loop to read frames from the input and send them to the RTSP server
    AVPacket packet;
    av_init_packet(&packet);

    while (av_read_frame(format_ctx, &packet) >= 0) {
        // Set the stream index for the packet
        packet.stream_index = 0;

        // Write the packet to the RTSP server
        if (av_write_frame(rtsp_ctx, &packet) < 0) {
            av_log(nullptr, AV_LOG_ERROR, "Error writing frame to RTSP server\n");
            break;
        }

        // Free the packet for the next iteration
        av_packet_unref(&packet);

        // Sleep to control the rate (optional)
        av_usleep(10000); // 10ms delay
    }

    // Write the trailer to the RTSP server
    av_write_trailer(rtsp_ctx);

    // Close the RTSP server and free resources
    avio_close(rtsp_ctx->pb);
    avformat_free_context(rtsp_ctx);
    avformat_close_input(&format_ctx);

    return 0;
}
