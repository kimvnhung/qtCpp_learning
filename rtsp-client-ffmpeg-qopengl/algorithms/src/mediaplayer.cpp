#include "mediaplayer.h"

#include "videoconsumer.h"
#include "audioconsumer.h"
#include "ffmpegdecoder.h"

#include "log.h"
#include "videosynchronizer.h"
#include "audiosynchronizer.h"

#define RENDER_LATENCY_MS 100
#define VIDEO_BUFFER_SIZE 1024
#define AUDIO_BUFFER_SIZE 1024

MediaPlayer::MediaPlayer()
    : audioOutput(std::make_shared<AudioConsumer>())
    , playbackClock(new PlaybackClock())
    , videoBuffer(std::make_shared<VideoFrameQueue>(VIDEO_BUFFER_SIZE))
    , audioBuffer(std::make_shared<AudioFrameQueue>(AUDIO_BUFFER_SIZE))
    , videoSynchronizer(new  VideoSynchronizer(playbackClock, videoBuffer))
    , audioSynchronizer(new AudioSynchronizer(audioOutput, playbackClock, audioBuffer))
    , decoder(nullptr)
    , scanTimer(new QTimer(this))
{

    scanTimer->setInterval(RENDER_LATENCY_MS);
    connect(scanTimer, &QTimer::timeout, this, &MediaPlayer::scan);
}

MediaPlayer::~MediaPlayer()
{
    stop();

    if (decoder)
    {
        delete decoder;
        decoder = nullptr;
    }

    if (audioOutput)
    {
        audioOutput.reset();
    }
}

bool MediaPlayer::open(QString url)
{
    LOGD();

    if (decoder)
    {
        delete decoder;
        decoder = nullptr;
    }

    decoder = new FFmpegDecoder();
    ffmpeg::FFmpegProducer producer;

    if (ffmpeg::open_input(
                producer,
                QString(url) == "live_stream" ? ffmpeg::LIVE_STREAM : ffmpeg::FILE_STREAM))
    {
        LOGD() << "Opened input source: " << url;
    }
    else
    {
        LOGW() << "Failed to open input source: " << url;
        return false;
    }

    decoder->start(producer, videoBuffer, audioBuffer);
    emit durationChanged();
    return true;
}

int MediaPlayer::fps() const
{
    return m_fps;
}

int MediaPlayer::currentTime() const
{
    return playbackClock->currentTime();
}

int MediaPlayer::duration() const
{
    return decoder ? decoder->duration() : 0;
}

AudioConsumer *MediaPlayer::getAudioOutput() const
{
    return audioOutput.get();
}

void MediaPlayer::play()
{
    LOGD();

    if (scanTimer && !scanTimer->isActive())
    {
        scanTimer->start();
    }

    playbackClock->play();
    emit playingChanged();
}
\
void MediaPlayer::pause()
{
    LOGD();

    if (scanTimer && scanTimer->isActive())
    {
        scanTimer->stop();
    }

    playbackClock->pause();
    emit playingChanged();
}

void MediaPlayer::stop()
{
    LOGD();

    if (scanTimer && scanTimer->isActive())
    {
        scanTimer->stop();
    }

    playbackClock->reset();

    if (audioOutput)
    {
        audioOutput->consume(std::make_shared<AudioFrame>().get());
    }

    for (auto output : videoOutputs)
    {
        if (output)
        {
            output->consume(std::make_shared<VideoFrame>().get());
        }
    }

    emit playingChanged();
    emit currentTimeChanged();
}

void MediaPlayer::seek(double position)
{
    LOGD() << "Seeking to position: " << position;
    playbackClock->seek(position);
}

void MediaPlayer::addVideoOutput(IFrameConsumer * output)
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

void MediaPlayer::scan()
{
    if (videoSynchronizer)
    {
        QThreadPool::globalInstance()->start([this]()
        {
            videoSynchronizer->processNext();
        });
    }

    if (audioSynchronizer)
    {
        QThreadPool::globalInstance()->start([this]()
        {
            audioSynchronizer->processNext();
        });
    }

    emit currentTimeChanged();
}
