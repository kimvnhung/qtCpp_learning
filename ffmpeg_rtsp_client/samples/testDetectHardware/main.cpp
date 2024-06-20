extern "C" {
    #include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/hwcontext.h>
}

static enum AVPixelFormat get_hw_pix_fmt(AVCodecContext *ctx, const enum AVPixelFormat *pix_fmts) {
    while (*pix_fmts != AV_PIX_FMT_NONE) {
        if (*pix_fmts == AV_PIX_FMT_CUDA || *pix_fmts == AV_PIX_FMT_VAAPI || *pix_fmts == AV_PIX_FMT_DXVA2_VLD) {
            return *pix_fmts;
        }
        pix_fmts++;
    }
    fprintf(stderr, "Failed to get HW surface format.\n");
    return AV_PIX_FMT_NONE;
}

int init_hw_decoder(AVCodecContext *ctx, enum AVHWDeviceType type) {
    int err;
    AVBufferRef *hw_device_ctx = NULL;

    if ((err = av_hwdevice_ctx_create(&hw_device_ctx, type, NULL, NULL, 0)) < 0) {
        fprintf(stderr, "Failed to create specified HW device.\n");
        return err;
    }
    ctx->hw_device_ctx = av_buffer_ref(hw_device_ctx);
    av_buffer_unref(&hw_device_ctx);
    return err;
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

    // Detect the hardware type
    enum AVHWDeviceType hw_type = AV_HWDEVICE_TYPE_NONE;
    const char *hw_type_name = NULL;
    while ((hw_type = av_hwdevice_iterate_types(hw_type)) != AV_HWDEVICE_TYPE_NONE) {
        hw_type_name = av_hwdevice_get_type_name(hw_type);
        printf("Found hardware type: %s\n", hw_type_name);

        // Break if the hardware type is suitable for current platform
        if (/*hw_type == AV_HWDEVICE_TYPE_CUDA
            || hw_type == AV_HWDEVICE_TYPE_VAAPI
            || */hw_type == AV_HWDEVICE_TYPE_DXVA2
            || hw_type == AV_HWDEVICE_TYPE_D3D11VA
            || hw_type == AV_HWDEVICE_TYPE_QSV
            || hw_type == AV_HWDEVICE_TYPE_OPENCL
            || hw_type == AV_HWDEVICE_TYPE_VULKAN) {
            printf("Use hardware type: %s\n", hw_type_name);
            break;
        }
    }

    if (hw_type == AV_HWDEVICE_TYPE_NONE) {
        fprintf(stderr, "No supported hardware device found.\n");
        return -1;
    }

    // Initialize hardware decoder
    if (init_hw_decoder(codec_context, hw_type) < 0) {
        fprintf(stderr, "Failed to initialize hardware decoder.\n");
        return -1;
    }

    codec_context->get_format = get_hw_pix_fmt;

    if (avcodec_open2(codec_context, codec, NULL) < 0) {
        fprintf(stderr, "Could not open codec\n");
        return -1;
    }

    AVPacket *packet = av_packet_alloc();
    AVFrame *frame = av_frame_alloc();
    AVFrame *hw_frame = av_frame_alloc();

    while (av_read_frame(format_context, packet) >= 0) {
        if (packet->stream_index == video_stream_index) {
            if (avcodec_send_packet(codec_context, packet) >= 0) {
                while (avcodec_receive_frame(codec_context, frame) >= 0) {
                    if (frame->format == codec_context->pix_fmt) {
                        // This is a hardware frame
                        av_hwframe_transfer_data(hw_frame, frame, 0);
                        printf("Hardware frame decoded\n");
                    } else {
                        printf("Software frame decoded\n");
                    }
                }
            }
        }
        av_packet_unref(packet);
    }

    av_frame_free(&frame);
    av_frame_free(&hw_frame);
    av_packet_free(&packet);
    avcodec_free_context(&codec_context);
    avformat_close_input(&format_context);

    return 0;
}
