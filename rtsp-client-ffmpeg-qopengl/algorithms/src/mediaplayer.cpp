#include "mediaplayer.h"

#include "log.h"

MediaPlayer::MediaPlayer()
    : audioOutput(new AudioConsumer())
    , decoder(nullptr)
    , state(State::Stopped)
{
    audioOutput->setName("AudioConsumer");
    audioOutput->start();
}

MediaPlayer::~MediaPlayer()
{
    if (decoder)
    {
        delete decoder;
        decoder = nullptr;
    }

    if (audioOutput)
    {
        audioOutput->stop();
        audioOutput->wait();
        delete audioOutput;
        audioOutput = nullptr;
    }
}

bool MediaPlayer::open(const char* url)
{
    LOGD();
    return true;
}

void MediaPlayer::play()
{
    LOGD();
    state = State::Playing;
}
\
void MediaPlayer::pause()
{
    LOGD();
    state = State::Paused;
}

void MediaPlayer::stop()
{
    LOGD();
    state = State::Stopped;
}

void MediaPlayer::seek(double position)
{
    LOGD() << "Seeking to position: " << position;
}

void MediaPlayer::addVideoOutput(VideoConsumer* output)
{
    if (output)
    {
        videoOutputs.push_back(output);
    }
}

bool MediaPlayer::isPlaying() const
{
    return state == State::Playing;
}
