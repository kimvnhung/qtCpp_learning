#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QObject>
#include <QTimer>

#include <memory>
#include <vector>

class VideoConsumer;
class AudioConsumer;
class PlaybackClock;
class FFmpegDecoder;
class VideoSynchronizer;
class AudioSynchronizer;
class VideoFrameQueue;
class AudioFrameQueue;
class MediaPlayer : public QObject
{
    Q_OBJECT
public:
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
    PlaybackClock *playbackClock;
    std::shared_ptr<VideoFrameQueue> videoBuffer;
    std::shared_ptr<AudioFrameQueue> audioBuffer;
    FFmpegDecoder *decoder;

    std::vector<VideoConsumer *> videoOutputs;
    std::shared_ptr<AudioConsumer> audioOutput;

    VideoSynchronizer *videoSynchronizer;
    AudioSynchronizer *audioSynchronizer;

    QTimer *scanTimer{nullptr};
    void scan();
};

#endif // MEDIAPLAYER_H
