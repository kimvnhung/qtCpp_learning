#include "mediaplayer.h"

#include "videoconsumer.h"
#include "audioconsumer.h"
#include "ffmpegdecoder.h"

#include "log.h"
#include "videosynchronizer.h"
#include "audiosynchronizer.h"

MediaPlayer::MediaPlayer()
    : audioOutput(std::make_shared<AudioConsumer>())
    , playbackClock(new PlaybackClock())
    , videoBuffer(std::make_shared<VideoFrameQueue>())
    , audioBuffer(std::make_shared<AudioFrameQueue>())
    , videoSynchronizer(new  VideoSynchronizer(playbackClock, videoBuffer))
    , audioSynchronizer(new AudioSynchronizer(audioOutput, playbackClock, audioBuffer))
    , decoder(nullptr)
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
        audioOutput.reset();
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
    playbackClock->play();
}
\
void MediaPlayer::pause()
{
    LOGD();
    playbackClock->pause();
}

void MediaPlayer::stop()
{
    LOGD();
    playbackClock->reset();
}

void MediaPlayer::seek(double position)
{
    LOGD() << "Seeking to position: " << position;
    playbackClock->seek(position);
}

void MediaPlayer::addVideoOutput(VideoConsumer* output)
{
    if (output)
    {
        videoOutputs.push_back(output);

        if (videoSynchronizer)
        {
            videoSynchronizer->registerConsumer(output);
        }
    }
}

bool MediaPlayer::isPlaying() const
{
    return playbackClock->isPlaying();
}
