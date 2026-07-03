#include "mediaplayer.h"

#include "videoconsumer.h"
#include "audioconsumer.h"
#include "ffmpegdecoder.h"

#include "log.h"
#include "videosynchronizer.h"
#include "audiosynchronizer.h"

#define RENDER_LATENCY_MS 100
#define VIDEO_BUFFER_SIZE 30
#define AUDIO_BUFFER_SIZE 50

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
    audioOutput->setName("AudioConsumer");
    audioOutput->start();

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
        audioOutput->stop();
        audioOutput->wait();
        audioOutput.reset();
    }


}

bool MediaPlayer::open(const char* url)
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
    return true;
}

void MediaPlayer::play()
{
    LOGD();

    if (scanTimer)
    {
        scanTimer->start();
    }

    playbackClock->play();
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
}

void MediaPlayer::stop()
{
    LOGD();

    if (scanTimer && scanTimer->isActive())
    {
        scanTimer->stop();
    }

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
}
