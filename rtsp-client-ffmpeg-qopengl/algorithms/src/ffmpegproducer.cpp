#include "ffmpegproducer.h"
#include "iframe.h"
#include "playbackclock.h"
#include "log.h"


#include <QMutex>
#include <mutex>
#include <thread>

// configurable macros
#define FFMPEGPRODUCER_VIDEO_FPS 25
#define FFMPEGPRODUCER_VIDEO_FRAME_MS (1000 / FFMPEGPRODUCER_VIDEO_FPS)
#define FFMPEGPRODUCER_AUDIO_FRAME_MS 23

namespace ffmpeg
{
    bool open_input(FFmpegProducer& producer, ProduceMode mode)
    {
        producer = FFmpegProducer(mode);
        return true;
    }

    bool read_frame(FFmpegProducer* producer, std::shared_ptr<IFrame> &frame, StreamIndex streamIndex)
    {
        if (!producer)
        {
            LOGW() << "Producer is null.";
            return false;
        }

        return producer->nextFrame(streamIndex, frame);
    }

    class FrameGenerator
    {
    public:
        FrameGenerator(int videoFps, int audioFps, int64_t durationMs)
            : m_videoFps(videoFps)
            , m_audioFps(audioFps)
            , m_durationMs(durationMs)
            , m_videoFrameCount(0)
            , m_audioFrameCount(0)
            , m_idCounter(0)
        {
            m_videoFrameIntervalMs = 1000 / m_videoFps;
            m_audioFrameIntervalMs = 1000 / m_audioFps;
        }

        bool nextVideoFrame(VideoFrame& frame)
        {
            if (m_videoFrameCount * m_videoFrameIntervalMs >= m_durationMs)
            {
                return false; // No more video frames to generate
            }

            frame = VideoFrame(nextId(), m_videoFrameCount++ * m_videoFrameIntervalMs, 1920, 1080);
            return true;
        }

        bool nextAudioFrame(AudioFrame& frame)
        {
            if (m_audioFrameCount * m_audioFrameIntervalMs >= m_durationMs)
            {
                return false; // No more audio frames to generate
            }

            frame = AudioFrame(nextId(), m_audioFrameCount++ * m_audioFrameIntervalMs, 48000, 2);
            return true;
        }
    private:
        int m_videoFps;
        int m_audioFps;
        int m_videoFrameIntervalMs;
        int m_audioFrameIntervalMs;
        int64_t m_durationMs;

        int m_videoFrameCount;
        int m_audioFrameCount;

        std::mutex m_mutex; // To protect access to m_idCounter
        int m_idCounter;
        int nextId()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_idCounter++;
        }
    };

    FFmpegProducer::FFmpegProducer(ProduceMode mode)
        : mode(mode)
        , frameGenerator(new FrameGenerator(FFMPEGPRODUCER_VIDEO_FPS, 1000 / FFMPEGPRODUCER_AUDIO_FRAME_MS,
                                            20 * 60000)) // Example: 20 minutes duration
    {
        if (mode == LIVE_STREAM)
        {
            startLive();
        }
    }

    FFmpegProducer::~FFmpegProducer()
    {

    }

    void FFmpegProducer::startLive()
    {
        if (mode != LIVE_STREAM)
        {
            return;
        }

        streamClock = new PlaybackClock();
        streamClock->play();
    }

    void FFmpegProducer::stopLive()
    {
        if (streamClock)
        {
            streamClock->reset();
            delete streamClock;
            streamClock = nullptr;
        }
    }

    ProduceMode FFmpegProducer::getMode() const
    {
        return mode;
    }

    bool FFmpegProducer::nextFrame(StreamIndex index, std::shared_ptr<IFrame> &frame)
    {
        if (frameGenerator == nullptr)
        {
            LOGW() << "Frame generator is not initialized.";
            return false;
        }

        if (index == VIDEO_STREAM)
        {
            VideoFrame videoFrame;
            bool hasNext = frameGenerator->nextVideoFrame(videoFrame);
            // LOGD() << "Mode: " << (mode == LIVE_STREAM ? "LIVE_STREAM" : "FILE_STREAM")
            //        << ", VideoFrame ID: " << videoFrame.id()
            //        << ", PTS: " << videoFrame.getPts()
            //        << ", Width: " << videoFrame.width()
            //        << ", Height: " << videoFrame.height()
            //        << ", hasNext: " << hasNext;

            if (mode == LIVE_STREAM)
            {
                // Wait until the next frame should be produced based on the playback clock
                int64_t currentTime = streamClock->currentTime();
                int64_t frameTime = videoFrame.getPts();

                if (frameTime > currentTime + 5) // Allow a small threshold to avoid busy waiting
                {
                    int64_t waitTime = frameTime - currentTime;
                    // Using thread sleep for blocking current thread until the next frame is due
                    std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));
                    frame = std::make_shared<VideoFrame>(videoFrame);
                }
            }
            else
            {
                frame = std::make_shared<VideoFrame>(videoFrame);
            }

            return hasNext;
        }
        else if (index == AUDIO_STREAM)
        {
            AudioFrame audioFrame;
            bool hasNext = frameGenerator->nextAudioFrame(audioFrame);

            if (mode == LIVE_STREAM)
            {
                // Wait until the next frame should be produced based on the playback clock
                int64_t currentTime = streamClock->currentTime();
                int64_t frameTime = audioFrame.getPts();

                if (frameTime > currentTime + 5) // Allow a small threshold to avoid busy waiting
                {
                    int64_t waitTime = frameTime - currentTime;
                    // Using thread sleep for blocking current thread until the next frame is due
                    std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));
                    frame = std::make_shared<AudioFrame>(audioFrame);
                }
            }
            else
            {
                frame = std::make_shared<AudioFrame>(audioFrame);
            }

            return hasNext;
        }
        else
        {
            LOGW() << "Unknown stream index: " << index;
            return false;
        }
    }

}

