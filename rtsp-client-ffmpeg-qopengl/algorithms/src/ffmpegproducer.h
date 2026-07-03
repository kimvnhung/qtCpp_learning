#ifndef FFMPEGPRODUCER_H
#define FFMPEGPRODUCER_H

#include "iframe.h"

#include <condition_variable>

class PlaybackClock;

namespace ffmpeg
{
    class FrameGenerator;
    enum ProduceMode
    {
        LIVE_STREAM,
        FILE_STREAM,
    };

    enum StreamIndex
    {
        VIDEO_STREAM = 0,
        AUDIO_STREAM = 1,
    };

    class FFmpegProducer
    {
    public:
        FFmpegProducer(ProduceMode mode = FILE_STREAM);
        ~FFmpegProducer();

        ProduceMode getMode() const;

        bool nextFrame(StreamIndex index, std::shared_ptr<IFrame> &frame);
        int getDuration() const;
    private:
        ProduceMode mode;
        PlaybackClock *streamClock{nullptr};
        FrameGenerator *frameGenerator{nullptr};
        void startLive();
        void stopLive();

    };

    // In real case, mode should depend on shecma of the input source
    bool open_input(FFmpegProducer& producer, ProduceMode mode);
    bool read_frame(FFmpegProducer* producer, std::shared_ptr<IFrame> &frame, StreamIndex streamIndex);
}

#endif // FFMPEGPRODUCER_H
