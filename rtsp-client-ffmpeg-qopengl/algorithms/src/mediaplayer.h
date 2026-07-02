#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include "playbackclock.h"
#include "ffmpegdecoder.h"
#include "videoconsumer.h"
#include "audioconsumer.h"

#include <vector>

class MediaPlayer
{
public:
    enum class State
    {
        Stopped,
        Playing,
        Paused
    };

    MediaPlayer();
    ~MediaPlayer();

    bool open(const char* url);
    void play();
    void pause();
    void stop();
    void seek(double position);

    void addVideoOutput(VideoConsumer* output);

    bool isPlaying() const;

private:
    State state;
    PlaybackClock playbackClock;
    FFmpegDecoder *decoder;

    std::vector<VideoConsumer *> videoOutputs;
    AudioConsumer *audioOutput;
};

#endif // MEDIAPLAYER_H
