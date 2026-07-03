#include "ffmpegdecoder.h"

#include "log.h"
#include "iframe.h"

void FFmpegDecoder::start(ffmpeg::FFmpegProducer& producer,
                          std::shared_ptr<VideoFrameQueue> videoQueue,
                          std::shared_ptr<AudioFrameQueue> audioQueue)
{
    this->producer = std::make_shared<ffmpeg::FFmpegProducer>(producer);
    this->videoQueue = videoQueue;
    this->audioQueue = audioQueue;

    m_running = true;

    threadPool.setMaxThreadCount(2);
    threadPool.start([this]() { decode(ffmpeg::VIDEO_STREAM); });
    threadPool.start([this]() { decode(ffmpeg::AUDIO_STREAM); });
}

void FFmpegDecoder::stop()
{
    m_running = false;
    threadPool.clear();
    threadPool.waitForDone();
}

void FFmpegDecoder::decode(ffmpeg::StreamIndex index)
{
    LOGD() << "Starting decoding thread for stream index: " << index;

    while (m_running)
    {
        if (producer == nullptr)
        {
            LOGW() << "Producer is null.";
            // Sleep to wait for the producer to be initialized
            QThread::msleep(10);
            continue;
        }

        std::shared_ptr<IFrame> frame;

        if (!ffmpeg::read_frame(producer.get(), frame, index))
        {
            LOGD() << "Failed to read frame from stream " << index;
            break;
        }

        if (index == ffmpeg::VIDEO_STREAM)
        {
            auto videoFrame = std::dynamic_pointer_cast<VideoFrame>(frame);

            if (videoFrame)
            {
                videoQueue->push(videoFrame);
            }
            else
            {
                LOGW() << "Failed to cast IFrame to VideoFrame";
            }
        }
        else if (index == ffmpeg::AUDIO_STREAM)
        {
            auto audioFrame = std::dynamic_pointer_cast<AudioFrame>(frame);

            if (audioFrame)
            {
                audioQueue->push(audioFrame);
            }
            else
            {
                LOGW() << "Failed to cast IFrame to AudioFrame";
            }
        }
        else
        {
            LOGW() << "Unknown stream index: " << index;
        }
    }
}

