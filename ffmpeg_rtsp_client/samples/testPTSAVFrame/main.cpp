extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
}

int main(int argc, char *argv[]) {
    const char *input_filename = "D:\\Codes\\NGS_documents\\videoplayback.mp4";

    AVFormatContext *format_context = avformat_alloc_context();
    if (!format_context) {
        fprintf(stderr, "Could not allocate format context\n");
        return -1;
    }

    if (avformat_open_input(&format_context, input_filename, NULL, NULL) < 0) {
        fprintf(stderr, "Could not open input file '%s'\n", input_filename);
        return -1;
    }

    if (avformat_find_stream_info(format_context, NULL) < 0) {
        fprintf(stderr, "Could not find stream information\n");
        return -1;
    }

    const AVCodec *codec = NULL;
    int video_stream_index = av_find_best_stream(format_context, AVMEDIA_TYPE_VIDEO, -1, -1, &codec, 0);
    if (video_stream_index < 0) {
        fprintf(stderr, "Could not find video stream in the input file\n");
        return -1;
    }

    AVStream *video_stream = format_context->streams[video_stream_index];

    AVCodecContext *codec_context = avcodec_alloc_context3(codec);
    if (!codec_context) {
        fprintf(stderr, "Could not allocate codec context\n");
        return -1;
    }

    if (avcodec_parameters_to_context(codec_context, video_stream->codecpar) < 0) {
        fprintf(stderr, "Could not initialize codec context\n");
        return -1;
    }

    if (avcodec_open2(codec_context, codec, NULL) < 0) {
        fprintf(stderr, "Could not open codec\n");
        return -1;
    }

    AVPacket *packet = av_packet_alloc();
    AVFrame *frame = av_frame_alloc();

    while (av_read_frame(format_context, packet) >= 0) {
        if (packet->stream_index == video_stream_index) {
            if (avcodec_send_packet(codec_context, packet) >= 0) {
                while (avcodec_receive_frame(codec_context, frame) >= 0) {
                    if (frame->pts != AV_NOPTS_VALUE) {
                        int64_t pts = frame->pts;
                        AVRational time_base = video_stream->time_base;
                        // Show pts and time_base raw value
                        printf("PTS: %" PRId64 " Time Base: %d/%d\n", pts, time_base.num, time_base.den);
                        double time_in_seconds = pts * av_q2d(time_base);
                        printf("Frame PTS: %" PRId64 " Time: %f seconds\n", pts, time_in_seconds);
                    } else {
                        printf("Frame does not have a valid PTS\n");
                    }
                }
            }
        }
        av_packet_unref(packet);
    }

    av_frame_free(&frame);
    av_packet_free(&packet);
    avcodec_free_context(&codec_context);
    avformat_close_input(&format_context);

    return 0;
}
