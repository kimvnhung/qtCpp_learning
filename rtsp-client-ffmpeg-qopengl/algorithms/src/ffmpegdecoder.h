#ifndef FFMPEGDECODER_H
#define FFMPEGDECODER_H

#include "iframequeue.h"
#include "ffmpegproducer.h"

#include <QThread>
#include <QThreadPool>

class FFmpegDecoder
{
public:
    void start(ffmpeg::FFmpegProducer& producer,
               std::shared_ptr<VideoFrameQueue> videoQueue,
               std::shared_ptr<AudioFrameQueue> audioQueue);

    void stop();

    int duration() const;
private:
    std::shared_ptr<VideoFrameQueue> videoQueue;
    std::shared_ptr<AudioFrameQueue> audioQueue;
    std::shared_ptr<ffmpeg::FFmpegProducer> producer;

    bool m_running{false};
    QThreadPool threadPool;

private:
    void decode(ffmpeg::StreamIndex index);
};

#endif // FFMPEGDECODER_H
