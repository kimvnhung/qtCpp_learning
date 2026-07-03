#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QObject>
#include <QTimer>
#include <QQmlEngine>

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
class IFrameConsumer;
class MediaPlayer : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(bool playing READ isPlaying NOTIFY playingChanged)
    Q_PROPERTY(int fps READ fps NOTIFY fpsChanged)
    Q_PROPERTY(AudioConsumer* audioOutput READ getAudioOutput CONSTANT)
    Q_PROPERTY(int currentTime READ currentTime NOTIFY currentTimeChanged)
    Q_PROPERTY(int duration READ duration NOTIFY durationChanged)

public:
    MediaPlayer();
    ~MediaPlayer();

    int fps() const;
    int currentTime() const;
    int duration() const;
    AudioConsumer *getAudioOutput() const;

    Q_INVOKABLE bool open(QString url);
    Q_INVOKABLE void play();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void seek(double position);

    Q_INVOKABLE void addVideoOutput(IFrameConsumer* output);

    bool isPlaying() const;
signals:
    void playingChanged();
    void fpsChanged();
    void currentTimeChanged();
    void durationChanged();
private:
    PlaybackClock *playbackClock;
    std::shared_ptr<VideoFrameQueue> videoBuffer;
    std::shared_ptr<AudioFrameQueue> audioBuffer;
    FFmpegDecoder *decoder;

    std::vector<IFrameConsumer *> videoOutputs;
    std::shared_ptr<AudioConsumer> audioOutput;

    VideoSynchronizer *videoSynchronizer;
    AudioSynchronizer *audioSynchronizer;

    QTimer *scanTimer{nullptr};
    void scan();
private:
    int m_fps{0};
};

#endif // MEDIAPLAYER_H
