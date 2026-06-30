#pragma once

#include <QObject>
#include <memory>
#include <vector>
#include <atomic>

namespace camera::ffmpeg { class FFmpegDecoder; }
namespace camera::core { class TripleBufferFrameQueue; class IFrame; }
namespace camera::renderer { class VideoItem; }

class DemoBridge : public QObject {
    Q_OBJECT
    Q_PROPERTY(int fps READ fps NOTIFY infoChanged)
    Q_PROPERTY(int width READ width NOTIFY infoChanged)
    Q_PROPERTY(int height READ height NOTIFY infoChanged)
    Q_PROPERTY(QString codec READ codec NOTIFY infoChanged)
public:
    explicit DemoBridge(QObject* parent = nullptr);
    ~DemoBridge() override;

    Q_INVOKABLE void start(const QString& url);
    Q_INVOKABLE void stop();
    Q_INVOKABLE void reconnect();

    void addVideoItem(QObject* item);

    int fps() const { return m_fps; }
    int width() const { return m_width; }
    int height() const { return m_height; }
    QString codec() const { return m_codec; }

signals:
    void infoChanged();

private:
    void pumpLoop();

    std::unique_ptr<camera::ffmpeg::FFmpegDecoder> m_decoder;
    std::shared_ptr<camera::core::TripleBufferFrameQueue> m_queue;
    std::vector<QObject*> m_items;
    std::atomic<bool> m_running{false};
    std::thread m_thread;
    std::mutex m_mutex;
    QString m_url;

    int m_fps = 0;
    int m_width = 0;
    int m_height = 0;
    QString m_codec;
};
