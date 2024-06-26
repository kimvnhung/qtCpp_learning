#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/dict.h>

void print_ts_durations(const char *url) {
    AVFormatContext *formatContext = NULL;
    const AVInputFormat *inputFormat = av_find_input_format("hls");

    if (!inputFormat) {
        fprintf(stderr, "Could not find input format 'hls'\n");
        return;
    }

    if (avformat_open_input(&formatContext, url, inputFormat, NULL) != 0) {
        fprintf(stderr, "Could not open source file %s\n", url);
        return;
    }

    if (avformat_find_stream_info(formatContext, NULL) < 0) {
        fprintf(stderr, "Could not find stream information\n");
        avformat_close_input(&formatContext);
        return;
    }

    AVStream *video_stream = NULL;
    for (unsigned int i = 0; i < formatContext->nb_streams; i++) {
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_stream = formatContext->streams[i];
            break;
        }
    }

    if (!video_stream) {
        fprintf(stderr, "Could not find video stream\n");
        avformat_close_input(&formatContext);
        return;
    }

    AVDictionaryEntry *tag = NULL;
    while ((tag = av_dict_get(formatContext->metadata, "variant_bitrate", tag, AV_DICT_IGNORE_SUFFIX))) {
        printf("Variant bitrate: %s\n", tag->value);
    }

    AVPacket packet;
    av_init_packet(&packet);

    int segment_index = 0;
    while (av_read_frame(formatContext, &packet) >= 0) {
        if (packet.stream_index == video_stream->index) {
            printf("Segment %d:\n", segment_index++);

            AVFormatContext *tsFormatContext = NULL;
            if (avformat_open_input(&tsFormatContext, packet.data, NULL, NULL) != 0) {
                fprintf(stderr, "Could not open segment file %s\n", packet.filename);
                av_packet_unref(&packet);
                continue;
            }

            if (avformat_find_stream_info(tsFormatContext, NULL) < 0) {
                fprintf(stderr, "Could not find stream information for segment %s\n", packet.filename);
                avformat_close_input(&tsFormatContext);
                av_packet_unref(&packet);
                continue;
            }

            int64_t duration = tsFormatContext->duration;
            printf("  Duration: %lld ms\n", duration);

            avformat_close_input(&tsFormatContext);
        }
        av_packet_unref(&packet);
    }

    avformat_close_input(&formatContext);
    avformat_network_deinit();
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <HLS URL>\n", argv[0]);
        return -1;
    }

    av_log_set_level(AV_LOG_INFO);
    // av_register_all();
    avformat_network_init();

    print_ts_durations(argv[1]);

    return 0;
}
