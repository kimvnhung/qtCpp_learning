// Full FFmpeg-based decoder implementation.
#include "ffmpeg/FFmpegDecoder.h"
#include <core/VideoFrame.h>
#include <core/VideoBuffer.h>
#include <core/MemoryPool.h>


extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
#include <libavutil/error.h>
#include <libavutil/samplefmt.h>
#include <libavutil/channel_layout.h>
}

#include <thread>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <chrono>
#include <atomic>
#include <iostream>

namespace camera::ffmpeg
{

// Utility: translate AV errors to string
    static std::string ff_err(int err)
    {
        char buf[256];
        av_strerror(err, buf, sizeof(buf));
        return std::string(buf);
    }

    // Thread-safe packet queue holding AVPacket* (each packet is a reference-owned copy)
    void PacketQueue::push(AVPacket* pkt)
    {
        std::unique_lock lock(mtx);
        q.emplace_back(pkt);
        cv.notify_one();
    }

    AVPacket *PacketQueue::pop(std::chrono::milliseconds wait)
    {
        std::unique_lock lock(mtx);

        if (q.empty())
        {
            cv.wait_for(lock, wait, [&]() { return !q.empty() || stopFlag; });
        }

        if (q.empty()) { return nullptr; }

        AVPacket* p = q.front();
        q.pop_front();
        return p;
    }

    void PacketQueue::flush()
    {
        std::unique_lock lock(mtx);

        for (auto p : q)
        {
            av_packet_free(&p);
        }

        q.clear();
    }

    void PacketQueue::stop() noexcept { stopFlag = true; cv.notify_all(); }


    core::IFrame::Timestamp FrameConverter::avts_to_timestamp(int64_t ts, AVRational tb)
    {
        if (ts == AV_NOPTS_VALUE) return core::VideoFrame::Timestamp{};

        // Convert to microseconds
        int64_t us = av_rescale_q(ts, tb, AVRational{1, 1000000});
        return core::IFrame::Timestamp(std::chrono::steady_clock::time_point(std::chrono::microseconds(us)));
    }

    core::VideoBufferPtr FrameConverter::convert(AVFrame* frame)
    {
        if (!frame) { return nullptr; }

        int w = frame->width;
        int h = frame->height;
        int planes = 0;
        size_t total = 0;
        std::vector<int> offsets;
        std::vector<int> strides;
        std::vector<int> sizes;

        if (frame->format == AV_PIX_FMT_YUV420P)
        {
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
        }
        else if (frame->format == AV_PIX_FMT_NV12)
        {
            planes = 2;
            int strideY = frame->linesize[0];
            int strideUV = frame->linesize[1];
            int sizeY = strideY * h;
            int sizeUV = strideUV * ((h + 1) / 2);
            offsets = {0, sizeY};
            strides = {strideY, strideUV};
            sizes = {sizeY, sizeUV};
            total = sizeY + sizeUV;
        }
        else
        {
            // unsupported format for now
            return nullptr;
        }

        auto buffer = std::make_shared<core::VideoBuffer>(total, offsets, strides, sizes);

        // Copy lines per plane to honor linesize vs width
        for (int p = 0; p < planes; ++p)
        {
            int planeH = (p == 0) ? h : ((h + 1) / 2);
            uint8_t *src = frame->data[p];
            int srcStride = frame->linesize[p];
            uint8_t *dst = buffer->planeDataMutable(p).data();
            int dstStride = strides[p];

            for (int r = 0; r < planeH; ++r)
            {
                memcpy(dst + r * dstStride, src + r * srcStride, static_cast<size_t>(dstStride));
            }
        }

        return buffer;
    }

    core::AudioBufferPtr FrameConverter::convertAudio(AVFrame* frame)
    {
        if (!frame) return nullptr;

        int channels = frame->channels;
        int sampleRate = frame->sample_rate;
        int nbSamples = frame->nb_samples;

        AVSampleFormat avfmt = static_cast<AVSampleFormat>(frame->format);
        int bytesPerSample = av_get_bytes_per_sample(avfmt);

        if (bytesPerSample <= 0) return nullptr;

        int bufferSize = av_samples_get_buffer_size(nullptr, channels, nbSamples, avfmt, 1);

        if (bufferSize <= 0) return nullptr;

        // Map to our SampleFormat
        core::SampleFormat fmt = core::SampleFormat::UNKNOWN;

        if (avfmt == AV_SAMPLE_FMT_S16 || avfmt == AV_SAMPLE_FMT_S16P) fmt = core::SampleFormat::S16;
        else if (avfmt == AV_SAMPLE_FMT_FLT || avfmt == AV_SAMPLE_FMT_FLTP) fmt = core::SampleFormat::FLT;

        auto buf = core::AudioBuffer::create(static_cast<size_t>(bufferSize), sampleRate, channels, nbSamples, fmt);

        // Prepare src pointers
        const uint8_t* srcPtr[AV_NUM_DATA_POINTERS] = {0};

        for (int i = 0; i < AV_NUM_DATA_POINTERS; ++i) srcPtr[i] = frame->data[i];

        // Destination pointer(s) - single interleaved buffer
        uint8_t* dst = buf->data();
        uint8_t* dstPtr[1] = { dst };

        // av_samples_copy will handle planar->interleaved copying when dst layout is interleaved
        int ret = av_samples_copy(dstPtr, srcPtr, 0, 0, nbSamples, channels, avfmt);

        if (ret < 0)
        {
            return nullptr;
        }

        return buf;
    }


    DecoderWorker::DecoderWorker(AVCodecContext* ctx, AVStream* stream, PacketQueue* q,
                                 std::shared_ptr<core::IFrameQueue> out)
        : codec(ctx), inStream(stream), pktQueue(q), outQueue(std::move(out)), running(true) {}

    AudioDecoderWorker::AudioDecoderWorker(AVCodecContext* ctx, AVStream* stream, PacketQueue* q,
                                           std::shared_ptr<core::IAudioFrameQueue> out)
        : codec(ctx), inStream(stream), pktQueue(q), outQueue(std::move(out)), running(true) {}

    AudioDecoderWorker::~AudioDecoderWorker()
    {
        if (codec)
        {
            avcodec_free_context(&codec);
            codec = nullptr;
        }
    }

    void DecoderWorker::operator()()
    {
        AVFrame* frame = av_frame_alloc();

        while (running)
        {
            AVPacket* pkt = pktQueue->pop(std::chrono::milliseconds(200));

            if (!pkt)
            {
                // check running
                continue;
            }

            int ret = avcodec_send_packet(codec, pkt);

            if (ret < 0)
            {
                av_packet_free(&pkt);
                continue;
            }

            av_packet_free(&pkt);

            while (ret >= 0)
            {
                ret = avcodec_receive_frame(codec, frame);

                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) { break; }

                if (ret < 0) { break; }

                // Convert AVFrame to VideoFrame
                auto buffer = FrameConverter::convert(frame);
                core::FrameMetadata meta;
                meta.width = frame->width;
                meta.height = frame->height;
                meta.format = (frame->format == AV_PIX_FMT_NV12) ? core::PixelFormat::NV12 : core::PixelFormat::YUV420P;

                if (frame->pts != AV_NOPTS_VALUE)
                {
                    meta.pts = FrameConverter::avts_to_timestamp(frame->pts, inStream->time_base);
                }

                if (frame->pkt_dts != AV_NOPTS_VALUE)
                {
                    meta.dts = FrameConverter::avts_to_timestamp(frame->pkt_dts, inStream->time_base);
                }

                // duration if available
                if (frame->pkt_duration > 0)
                    meta.duration = std::chrono::microseconds(av_rescale_q(frame->pkt_duration, inStream->time_base, AVRational{1, 1000000}));
                // rotation and color info may be in side data or stream - omitted for brevity

                if (buffer)
                {
                    auto vframe = core::VideoFrame::create(buffer, meta);

                    if (vframe && outQueue) { outQueue->push(vframe); }
                }

                av_frame_unref(frame);
            }
        }

        av_frame_free(&frame);
    }

    void AudioDecoderWorker::operator()()
    {
        AVFrame* frame = av_frame_alloc();

        while (running)
        {
            AVPacket* pkt = pktQueue->pop(std::chrono::milliseconds(200));

            if (!pkt)
            {
                continue;
            }

            int ret = avcodec_send_packet(codec, pkt);

            if (ret < 0)
            {
                av_packet_free(&pkt);
                continue;
            }

            av_packet_free(&pkt);

            while (ret >= 0)
            {
                ret = avcodec_receive_frame(codec, frame);

                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) { break; }

                if (ret < 0) { break; }

                // Convert to AudioBuffer
                auto buffer = FrameConverter::convertAudio(frame);

                camera::core::AudioFrameMetadata meta;

                if (frame->pts != AV_NOPTS_VALUE)
                {
                    meta.pts = FrameConverter::avts_to_timestamp(frame->pts, inStream->time_base);
                }

                if (frame->pkt_duration > 0)
                    meta.duration = std::chrono::microseconds(av_rescale_q(frame->pkt_duration, inStream->time_base, AVRational{1, 1000000}));

                if (buffer && outQueue)
                {
                    auto afr = camera::core::AudioFrame::create(buffer, meta);
                    outQueue->push(afr);
                }

                av_frame_unref(frame);
            }
        }

        av_frame_free(&frame);
    }

    void AudioDecoderWorker::stop() noexcept { running = false; }

    void DecoderWorker::stop() noexcept { running = false; }

// PacketReader: opens input, reads packets, handles reconnect and interrupt/timeout
    PacketReader::PacketReader(AVFormatContext* fmt, PacketQueue* q, int stream_index)
        : m_fmt(fmt)
        , m_streamIndex(stream_index)
        , pktQueue(q)
        , m_timeoutMs(5000)
        , m_running(false) {}

    PacketReader::~PacketReader() { stop(); }

    bool PacketReader::start()
    {
        m_running = true;
        readerThread = std::thread(&PacketReader::run, this);
        return true;
    }

    void PacketReader::stop() noexcept
    {
        m_running = false;

        if (readerThread.joinable()) { readerThread.join(); }
    }

    void PacketReader::setTimeoutMs(int ms) { m_timeoutMs = ms; }

    int PacketReader::interrupt_cb(void* ctx)
    {
        auto self = static_cast<PacketReader *>(ctx);

        if (!self->m_running) { return 1; } // interrupt

        // if timeout exceeded, interrupt
        auto now = std::chrono::steady_clock::now();

        if (self->m_timeoutMs > 0)
        {
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - self->lastActivity).count();

            if (elapsed > self->m_timeoutMs) { return 1; }
        }

        return 0;
    }

    void PacketReader::run()
    {
        while (m_running)
        {
            AVPacket* pkt = av_packet_alloc();

            if (!m_fmt)
            {
                std::cerr << "Format context is null, cannot read packets\n";
                break;
            }

            int ret = av_read_frame(m_fmt, pkt);

            if (ret < 0)
            {
                av_packet_free(&pkt);
                std::cerr << "av_read_frame error: " << ff_err(ret) << "\n";
                break; // try reconnect
            }

            if (pkt->stream_index != m_streamIndex)
            {
                std::cerr << "Packet from stream " << pkt->stream_index << " ignored, expecting stream " << m_streamIndex << "\n";
                av_packet_free(&pkt);
                continue; // ignore other streams
            }

            lastActivity = std::chrono::steady_clock::now();
            // push packet (decoder required to unref/free it)
            pktQueue->push(pkt);
        }
    }

// FFmpegDecoder implementation using PacketReader and DecoderWorker.

    class FFmpegDecoderImpl
    {
    public:
        FFmpegDecoderImpl() = default;

        bool start(const core::DecoderConfig& cfg, std::shared_ptr<core::IFrameQueue> outQueue)
        {
            url = cfg.url;
            this->outQueue = outQueue;

            if (avformat_open_input(&formatCtx, url.c_str(), nullptr, nullptr) < 0)
            {
                std::cerr << "Failed to open input for probe: " << url << "\n";
                return false;
            }

            if (avformat_find_stream_info(formatCtx, nullptr) < 0)
            {
                avformat_close_input(&formatCtx);
                return false;
            }

            for (unsigned int i = 0; i < formatCtx->nb_streams; i++)
            {
                if (formatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
                {
                    streamIndex = i;
                    break;
                }
            }

            if (streamIndex == -1)
            {
                std::cerr << "No video stream found in input: " << url << "\n";
                return false;
            }

            AVStream* vs = formatCtx->streams[streamIndex];

            AVCodecContext* codec_ctx = avcodec_alloc_context3(nullptr);

            if (!codec_ctx) { avformat_close_input(&formatCtx); return false; }

            const AVCodec* dec = avcodec_find_decoder(vs->codecpar->codec_id);

            if (!dec) { avformat_close_input(&formatCtx); return false; }

            if (avcodec_parameters_to_context(codec_ctx, vs->codecpar) < 0) { avcodec_free_context(&codec_ctx); avformat_close_input(&formatCtx); return false; }

            if (avcodec_open2(codec_ctx, dec, nullptr) < 0) { avcodec_free_context(&codec_ctx); avformat_close_input(&formatCtx); return false; }

            // start packet queue, reader and decoder threads
            pktQueue = std::make_unique<PacketQueue>();
            reader = std::make_unique<PacketReader>(formatCtx, pktQueue.get(), streamIndex);
            reader->start();

            // start decoder worker thread
            decoderWorker = std::make_unique<DecoderWorker>(codec_ctx, vs, pktQueue.get(), outQueue);
            decoderThread = std::thread(std::ref(*decoderWorker));

                // if audio queue provided in cfg, find audio stream and start audio decoding
                if (cfg.audioQueue)
                {
                    int audioIndex = -1;

                    for (unsigned int i = 0; i < formatCtx->nb_streams; ++i)
                    {
                        if (formatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
                        {
                            audioIndex = static_cast<int>(i);
                            break;
                        }
                    }

                    if (audioIndex != -1)
                    {
                        AVStream* as = formatCtx->streams[audioIndex];
                        AVCodecContext* audio_ctx = avcodec_alloc_context3(nullptr);

                        const AVCodec* adec = avcodec_find_decoder(as->codecpar->codec_id);

                        if (adec && audio_ctx)
                        {
                            if (avcodec_parameters_to_context(audio_ctx, as->codecpar) >= 0 && avcodec_open2(audio_ctx, adec, nullptr) >= 0)
                            {
                                audioPktQueue = std::make_unique<PacketQueue>();
                                audioReader = std::make_unique<PacketReader>(formatCtx, audioPktQueue.get(), audioIndex);
                                audioReader->start();

                                audioDecoderWorker = std::make_unique<AudioDecoderWorker>(audio_ctx, as, audioPktQueue.get(), cfg.audioQueue);
                                audioDecoderThread = std::thread(std::ref(*audioDecoderWorker));
                            }
                            else
                            {
                                avcodec_free_context(&audio_ctx);
                            }
                        }
                    }
                }

            return true;
        }

        void stop() noexcept
        {
            if (reader)
            {
                reader->stop();
                reader.reset();
            }

            if (pktQueue)
            {
                pktQueue->stop();
                pktQueue->flush();
            }

            if (decoderWorker)
            {
                decoderWorker->stop();

                if (decoderThread.joinable()) { decoderThread.join(); }

                decoderWorker.reset();
            }

            // stop audio path if present
            if (audioReader)
            {
                audioReader->stop();
                audioReader.reset();
            }

            if (audioPktQueue)
            {
                audioPktQueue->stop();
                audioPktQueue->flush();
            }

            if (audioDecoderWorker)
            {
                audioDecoderWorker->stop();

                if (audioDecoderThread.joinable()) { audioDecoderThread.join(); }

                audioDecoderWorker.reset();
            }

            if (formatCtx)
            {
                avformat_close_input(&formatCtx);
                formatCtx = nullptr;
            }
        }

        void seek(int64_t timestamp, int streamIndex)
        {
            if (!formatCtx) { return; }

            av_seek_frame(formatCtx, streamIndex, timestamp, AVSEEK_FLAG_BACKWARD);

            // flush codec buffers by pushing null packet to decoder
            if (pktQueue) { pktQueue->flush(); }
        }

    private:
        std::string url;
        std::unique_ptr<PacketQueue> pktQueue;
        std::unique_ptr<PacketReader> reader;
        std::unique_ptr<DecoderWorker> decoderWorker;
        std::thread decoderThread;
        std::shared_ptr<core::IFrameQueue> outQueue;
        AVFormatContext *formatCtx = nullptr;
        int streamIndex = -1;
        // audio path
        std::unique_ptr<PacketQueue> audioPktQueue;
        std::unique_ptr<PacketReader> audioReader;
        std::unique_ptr<AudioDecoderWorker> audioDecoderWorker;
        std::thread audioDecoderThread;
    };

// FFmpegDecoder public wrapper
    FFmpegDecoder::FFmpegDecoder()
        : impl(std::make_unique<FFmpegDecoderImpl>())
    {
    }

    FFmpegDecoder::~FFmpegDecoder()
    {
        stop();
    }

    bool FFmpegDecoder::start(const camera::core::DecoderConfig& cfg, std::shared_ptr<camera::core::IFrameQueue> outQueue)
    {
        return impl->start(cfg, std::move(outQueue));
    }

    void FFmpegDecoder::stop() noexcept
    {
        if (impl) { impl->stop(); }
    }

} // namespace camera::ffmpeg
