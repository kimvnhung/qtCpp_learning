#pragma once

#include <QObject>
#include <memory>
#include <vector>
#include <atomic>
#include <thread>
#include <mutex>

namespace camera::ffmpeg { class FFmpegDecoder; }
namespace camera::core { class TripleBufferFrameQueue; class IFrame; class BlockingAudioFrameQueue; }
namespace camera::renderer { class VideoItem; class AudioPlayer; }

class MediaPlayer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int fps READ fps NOTIFY infoChanged)
    Q_PROPERTY(int width READ width NOTIFY infoChanged)
    Q_PROPERTY(int height READ height NOTIFY infoChanged)
    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)

public:
    explicit MediaPlayer(QObject* parent = nullptr);
    ~MediaPlayer() override;

    Q_INVOKABLE void start(const QString& url);
    Q_INVOKABLE void stop();
    Q_INVOKABLE void setVolume(qreal v);
    Q_INVOKABLE void addVideoItem(QObject* item);

    int fps() const { return m_fps; }
    int width() const { return m_width; }
    int height() const { return m_height; }
    QString url() const { return m_url; }
    void setUrl(const QString& u) { if (m_url == u) return; m_url = u; emit urlChanged(); }

signals:
    void infoChanged();
    void urlChanged();

private:
    void pumpLoop();

    std::unique_ptr<camera::ffmpeg::FFmpegDecoder> m_decoder;
    std::shared_ptr<camera::core::TripleBufferFrameQueue> m_queue;
    std::shared_ptr<camera::core::BlockingAudioFrameQueue> m_audioQueue;
    std::unique_ptr<camera::renderer::AudioPlayer> m_audioPlayer;

    std::vector<QObject*> m_items;
    std::atomic<bool> m_running{false};
    std::thread m_thread;
    std::mutex m_mutex;
    QString m_url;

    int m_fps = 0;
    int m_width = 0;
    int m_height = 0;
};
