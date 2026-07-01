// AudioPlayer.h
#pragma once

#include <QObject>
#include <QAudioOutput>
#include <memory>
#include <atomic>
#include <core/IAudioFrameQueue.h>

namespace camera::renderer {

class AudioPlayer : public QObject
{
    Q_OBJECT
public:
    explicit AudioPlayer(QObject* parent = nullptr);
    ~AudioPlayer() override;

    Q_INVOKABLE void start(std::shared_ptr<camera::core::IAudioFrameQueue> queue);
    Q_INVOKABLE void stop() noexcept;
    Q_INVOKABLE void setVolume(qreal v);

private:
    class AudioIODevice;

    std::shared_ptr<camera::core::IAudioFrameQueue> m_queue;
    QAudioOutput* m_output = nullptr;
    AudioIODevice* m_io = nullptr;
    std::atomic<bool> m_running{false};
};

} // namespace camera::renderer
