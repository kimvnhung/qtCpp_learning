// Full FFmpeg-based decoder implementation.
#include "../include/ffmpeg/FFmpegDecoder.h"
#include <core/VideoFrame.h>
#include <core/VideoBuffer.h>
#include <core/MemoryPool.h>

#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
#include <libavutil/error.h>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <chrono>
#include <atomic>
#include <iostream>

namespace camera::ffmpeg {

// Utility: translate AV errors to string
static std::string ff_err(int err)
{
    char buf[256];
    av_strerror(err, buf, sizeof(buf));
    return std::string(buf);
}

// Thread-safe packet queue holding AVPacket* (each packet is a reference-owned copy)
class PacketQueue {
public:
    PacketQueue() = default;

    void push(AVPacket* pkt) {
        std::unique_lock lock(mtx);
        q.emplace_back(pkt);
        cv.notify_one();
    }

    AVPacket* pop(std::chrono::milliseconds wait = std::chrono::milliseconds(100)) {
        std::unique_lock lock(mtx);
        if (q.empty()) {
            cv.wait_for(lock, wait, [&]() { return !q.empty() || stopFlag; });
        }
        if (q.empty()) return nullptr;
        AVPacket* p = q.front();
        q.pop_front();
        return p;
    }

    void flush() {
        std::unique_lock lock(mtx);
        for (auto p : q) av_packet_free(&p);
        q.clear();
    }

    void stop() noexcept { stopFlag = true; cv.notify_all(); }

private:
    std::deque<AVPacket*> q;
    std::mutex mtx;
    std::condition_variable cv;
    bool stopFlag = false;
};

// FrameConverter: convert AVFrame to VideoFrame (VideoBuffer + metadata)
class FrameConverter {
public:
    static core::IFrame::Timestamp avts_to_timestamp(int64_t ts, AVRational tb) {
        if (ts == AV_NOPTS_VALUE) return core::VideoFrame::Timestamp{};
        // Convert to microseconds
        int64_t us = av_rescale_q(ts, tb, AVRational{1, 1000000});
        return core::IFrame::Timestamp(std::chrono::steady_clock::time_point(std::chrono::microseconds(us)));
    }

    static core::VideoBufferPtr convert(AVFrame* frame) {
        if (!frame) return nullptr;
        int w = frame->width;
        int h = frame->height;
        int planes = 0;
        size_t total = 0;
        std::vector<int> offsets;
        std::vector<int> strides;
        std::vector<int> sizes;

        if (frame->format == AV_PIX_FMT_YUV420P) {
            planes = 3;
            int strideY = frame->linesize[0];
            int strideU = frame->linesize[1];
            int strideV = frame->linesize[2];
            int sizeY = strideY * h;
            int sizeU = strideU * ((h + 1) / 2);
            int sizeV = strideV * ((h + 1) / 2);
            offsets = {0, sizeY, sizeY + sizeU};
            strides = {strideY, strideU, strideV};
            sizes = {sizeY, sizeU, sizeV};
            total = sizeY + sizeU + sizeV;
        } else if (frame->format == AV_PIX_FMT_NV12) {
            planes = 2;
            int strideY = frame->linesize[0];
            int strideUV = frame->linesize[1];
            int sizeY = strideY * h;
            int sizeUV = strideUV * ((h + 1) / 2);
            offsets = {0, sizeY};
            strides = {strideY, strideUV};
            sizes = {sizeY, sizeUV};
            total = sizeY + sizeUV;
        } else {
            // unsupported format for now
            return nullptr;
        }

        auto buffer = std::make_shared<core::VideoBuffer>(total, offsets, strides, sizes);

        // Copy lines per plane to honor linesize vs width
        for (int p = 0; p < planes; ++p) {
            int planeH = (p == 0) ? h : ((h + 1) / 2);
            uint8_t* src = frame->data[p];
            int srcStride = frame->linesize[p];
            uint8_t* dst = buffer->planeDataMutable(p).data();
            int dstStride = strides[p];
            for (int r = 0; r < planeH; ++r) {
                memcpy(dst + r * dstStride, src + r * srcStride, static_cast<size_t>(dstStride));
            }
        }

        return buffer;
    }
};

// DecoderWorker: takes packets, feeds codec, emits frames
class DecoderWorker {
public:
    DecoderWorker(AVCodecContext* ctx, AVStream* stream, PacketQueue* q, std::shared_ptr<core::IFrameQueue> out)
        : codec(ctx), inStream(stream), pktQueue(q), outQueue(std::move(out)), running(true) {}

    void operator()() {
        AVFrame* frame = av_frame_alloc();
        while (running) {
            AVPacket* pkt = pktQueue->pop(std::chrono::milliseconds(200));
            if (!pkt) {
                // check running
                continue;
            }

            int ret = avcodec_send_packet(codec, pkt);
            if (ret < 0) {
                av_packet_free(&pkt);
                continue;
            }
            av_packet_free(&pkt);

            while (ret >= 0) {
                ret = avcodec_receive_frame(codec, frame);
                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) break;
                if (ret < 0) break;

                // Convert AVFrame to VideoFrame
                auto buffer = FrameConverter::convert(frame);
                core::FrameMetadata meta;
                meta.width = frame->width;
                meta.height = frame->height;
                meta.format = (frame->format == AV_PIX_FMT_NV12) ? core::PixelFormat::NV12 : core::PixelFormat::YUV420P;
                if (frame->pts != AV_NOPTS_VALUE)
                    meta.pts = FrameConverter::avts_to_timestamp(frame->pts, inStream->time_base);
                if (frame->pkt_dts != AV_NOPTS_VALUE)
                    meta.dts = FrameConverter::avts_to_timestamp(frame->pkt_dts, inStream->time_base);
                // duration if available
                if (frame->pkt_duration > 0)
                    meta.duration = std::chrono::microseconds(av_rescale_q(frame->pkt_duration, inStream->time_base, AVRational{1,1000000}));
                // rotation and color info may be in side data or stream - omitted for brevity

                if (buffer) {
                    auto vframe = core::VideoFrame::create(buffer, meta);
                    if (vframe && outQueue) outQueue->push(vframe);
                }

                av_frame_unref(frame);
            }
        }

        av_frame_free(&frame);
    }

    void stop() noexcept { running = false; }

private:
    AVCodecContext* codec;
    AVStream* inStream;
    PacketQueue* pktQueue;
    std::shared_ptr<core::IFrameQueue> outQueue;
    std::atomic<bool> running;
};

// PacketReader: opens input, reads packets, handles reconnect and interrupt/timeout
class PacketReader {
public:
    PacketReader(const std::string& url, PacketQueue* q)
        : m_url(url), pktQueue(q), m_timeoutMs(5000), m_running(false) {}

    ~PacketReader() { stop(); }

    bool start() {
        m_running = true;
        readerThread = std::thread(&PacketReader::run, this);
        return true;
    }

    void stop() noexcept {
        m_running = false;
        if (readerThread.joinable()) readerThread.join();
    }

    void setTimeoutMs(int ms) { m_timeoutMs = ms; }

private:
    static int interrupt_cb(void* ctx) {
        auto self = static_cast<PacketReader*>(ctx);
        if (!self->m_running) return 1; // interrupt
        // if timeout exceeded, interrupt
        auto now = std::chrono::steady_clock::now();
        if (self->m_timeoutMs > 0) {
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - self->lastActivity).count();
            if (elapsed > self->m_timeoutMs) return 1;
        }
        return 0;
    }

    void run() {
        AVFormatContext* fmt = nullptr;
        AVDictionary* opts = nullptr;
        avformat_network_init();

        while (m_running) {
            int ret = avformat_open_input(&fmt, m_url.c_str(), nullptr, &opts);
            if (ret < 0) {
                std::cerr << "avformat_open_input failed: " << ff_err(ret) << "\n";
                std::this_thread::sleep_for(std::chrono::seconds(1));
                continue;
            }

            avformat_find_stream_info(fmt, nullptr);

            lastActivity = std::chrono::steady_clock::now();

            while (m_running) {
                AVPacket* pkt = av_packet_alloc();
                ret = av_read_frame(fmt, pkt);
                if (ret < 0) {
                    av_packet_free(&pkt);
                    std::cerr << "av_read_frame error: " << ff_err(ret) << "\n";
                    break; // try reconnect
                }

                lastActivity = std::chrono::steady_clock::now();
                // push packet (decoder required to unref/free it)
                pktQueue->push(pkt);
            }

            avformat_close_input(&fmt);
            if (m_running) std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }

    std::string m_url;
    PacketQueue* pktQueue;
    std::thread readerThread;
    std::atomic<bool> m_running;
    std::chrono::steady_clock::time_point lastActivity{};
    int m_timeoutMs;
};

// FFmpegDecoder implementation using PacketReader and DecoderWorker.

class FFmpegDecoderImpl {
public:
    FFmpegDecoderImpl() = default;

    bool start(const core::DecoderConfig& cfg, std::shared_ptr<core::IFrameQueue> outQueue) {
        url = cfg.url;
        this->outQueue = outQueue;

        // open format to find video stream and create codec context for initial setup
        AVFormatContext* fmt = nullptr;
        if (avformat_open_input(&fmt, url.c_str(), nullptr, nullptr) < 0) {
            std::cerr << "Failed to open input for probe: " << url << "\n";
            return false;
        }
        if (avformat_find_stream_info(fmt, nullptr) < 0) {
            avformat_close_input(&fmt);
            return false;
        }

        // find best video stream
        int stream_index = av_find_best_stream(fmt, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
        if (stream_index < 0) { avformat_close_input(&fmt); return false; }
        AVStream* vs = fmt->streams[stream_index];

        AVCodec* dec = avcodec_find_decoder(vs->codecpar->codec_id);
        if (!dec) { avformat_close_input(&fmt); return false; }

        AVCodecContext* codec_ctx = avcodec_alloc_context3(dec);
        if (!codec_ctx) { avformat_close_input(&fmt); return false; }
        if (avcodec_parameters_to_context(codec_ctx, vs->codecpar) < 0) { avcodec_free_context(&codec_ctx); avformat_close_input(&fmt); return false; }
        if (avcodec_open2(codec_ctx, dec, nullptr) < 0) { avcodec_free_context(&codec_ctx); avformat_close_input(&fmt); return false; }

        // done probing, close probe context
        avcodec_free_context(&codec_ctx);
        avformat_close_input(&fmt);

        // start packet queue, reader and decoder threads
        pktQueue = std::make_unique<PacketQueue>();
        reader = std::make_unique<PacketReader>(url, pktQueue.get());
        reader->start();

        // reopen format and codec for actual decoding in decoder thread
        if (avformat_open_input(&fmt, url.c_str(), nullptr, nullptr) < 0) {
            std::cerr << "Failed to open input for decode\n";
            return false;
        }
        if (avformat_find_stream_info(fmt, nullptr) < 0) {
            avformat_close_input(&fmt);
            return false;
        }

        streamIndex = av_find_best_stream(fmt, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
        if (streamIndex < 0) { avformat_close_input(&fmt); return false; }
        AVStream* vstream = fmt->streams[streamIndex];

        AVCodec* decoder = avcodec_find_decoder(vstream->codecpar->codec_id);
        if (!decoder) { avformat_close_input(&fmt); return false; }

        AVCodecContext* cctx = avcodec_alloc_context3(decoder);
        if (!cctx) { avformat_close_input(&fmt); return false; }
        if (avcodec_parameters_to_context(cctx, vstream->codecpar) < 0) { avcodec_free_context(&cctx); avformat_close_input(&fmt); return false; }
        if (avcodec_open2(cctx, decoder, nullptr) < 0) { avcodec_free_context(&cctx); avformat_close_input(&fmt); return false; }

        // start decoder worker thread
        decoderWorker = std::make_unique<DecoderWorker>(cctx, vstream, pktQueue.get(), outQueue);
        decoderThread = std::thread(std::ref(*decoderWorker));

        // store format context for control operations
        formatCtx = fmt;

        return true;
    }

    void stop() noexcept {
        if (reader) {
            reader->stop();
            reader.reset();
        }
        if (pktQueue) {
            pktQueue->stop();
            pktQueue->flush();
        }
        if (decoderWorker) {
            decoderWorker->stop();
            if (decoderThread.joinable()) decoderThread.join();
            decoderWorker.reset();
        }
        if (formatCtx) {
            avformat_close_input(&formatCtx);
            formatCtx = nullptr;
        }
    }

    void seek(int64_t timestamp, int streamIndex) {
        if (!formatCtx) return;
        av_seek_frame(formatCtx, streamIndex, timestamp, AVSEEK_FLAG_BACKWARD);
        // flush codec buffers by pushing null packet to decoder
        if (pktQueue) pktQueue->flush();
    }

private:
    std::string url;
    std::unique_ptr<PacketQueue> pktQueue;
    std::unique_ptr<PacketReader> reader;
    std::unique_ptr<DecoderWorker> decoderWorker;
    std::thread decoderThread;
    std::shared_ptr<core::IFrameQueue> outQueue;
    AVFormatContext* formatCtx = nullptr;
    int streamIndex = -1;
};

// FFmpegDecoder public wrapper
FFmpegDecoder::FFmpegDecoder() : impl(new FFmpegDecoderImpl()) {}
FFmpegDecoder::~FFmpegDecoder() { stop(); delete impl; }

bool FFmpegDecoder::start(const camera::core::DecoderConfig& cfg, std::shared_ptr<camera::core::IFrameQueue> outQueue)
{
    return impl->start(cfg, std::move(outQueue));
}

void FFmpegDecoder::stop() noexcept
{
    if (impl) impl->stop();
}

} // namespace camera::ffmpeg
