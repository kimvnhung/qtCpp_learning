// FFmpegDecoder.h
#pragma once

#include <condition_variable>
#include <deque>
#include <memory>
#include <core/IDecoder.h>
#include <core/VideoBuffer.h>


class AVPacket;
class AVRational;
class AVFrame;

class AVCodecContext;
class AVFormatContext;
class AVStream;
namespace camera::ffmpeg
{

    class FFmpegDecoderImpl;


    class FFmpegDecoder : public camera::core::IDecoder
    {
    public:
        enum ErrorCode
        {
            SUCCESS = 0,
            INIT_FAILED = 1,
            DECODE_FAILED = 2,
            UNSUPPORTED_FORMAT = 3,
            UNKNOWN_ERROR = -1
        };

        FFmpegDecoder();
        ~FFmpegDecoder() override;

        bool start(const camera::core::DecoderConfig& cfg, std::shared_ptr<camera::core::IFrameQueue> outQueue) override;
        void stop() noexcept override;

    private:
        std::unique_ptr<FFmpegDecoderImpl> impl;
    };


    // Thread-safe packet queue holding AVPacket* (each packet is a reference-owned copy)
    class PacketQueue
    {
    public:
        PacketQueue() = default;

        void push(AVPacket* pkt);

        AVPacket *pop(std::chrono::milliseconds wait = std::chrono::milliseconds(100));

        void flush();
        void stop() noexcept ;

    private:
        std::deque<AVPacket *> q;
        std::mutex mtx;
        std::condition_variable cv;
        bool stopFlag = false;
    };


    // FrameConverter: convert AVFrame to VideoFrame (VideoBuffer + metadata)
    class FrameConverter
    {
    public:
        static core::IFrame::Timestamp avts_to_timestamp(int64_t ts, AVRational tb);
        static core::VideoBufferPtr convert(AVFrame* frame);
    };

    // DecoderWorker: takes packets, feeds codec, emits frames
    class DecoderWorker
    {
    public:
        DecoderWorker(AVCodecContext* ctx, AVStream* stream, PacketQueue* q, std::shared_ptr<core::IFrameQueue> out);
        void operator()();
        void stop() noexcept ;

    private:
        AVCodecContext *codec;
        AVStream *inStream;
        PacketQueue *pktQueue;
        std::shared_ptr<core::IFrameQueue> outQueue;
        std::atomic<bool> running;
    };

    // PacketReader: opens input, reads packets, handles reconnect and interrupt/timeout
    class PacketReader
    {
    public:
        PacketReader(AVFormatContext* fmt, PacketQueue* q, int stream_index);

        ~PacketReader();

        bool start();

        void stop() noexcept;

        void setTimeoutMs(int ms);

    private:
        static int interrupt_cb(void* ctx);

        void run();

        AVFormatContext *m_fmt;
        int m_streamIndex;
        PacketQueue *pktQueue;
        std::thread readerThread;
        std::atomic<bool> m_running;
        std::chrono::steady_clock::time_point lastActivity{};
        int m_timeoutMs;
    };

} // namespace camera::ffmpeg
