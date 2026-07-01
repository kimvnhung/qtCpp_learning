// AudioPlayer.cpp - QAudioOutput-based audio player
#include "renderer/AudioPlayer.h"
#include <QAudioFormat>
#include <QAudioDevice>
#include <QIODevice>
#include <QDebug>

namespace camera::renderer
{

    class AudioPlayer::AudioIODevice : public QIODevice
    {
    public:
        AudioIODevice(std::shared_ptr<camera::core::IAudioFrameQueue> q, QObject* parent = nullptr)
            : QIODevice(parent), m_queue(std::move(q))
        {}

        bool openMode(QIODevice::OpenMode mode)
        {
            return open(mode);
        }

        qint64 readData(char* data, qint64 maxlen) override
        {
            if (!m_queue) { return 0; }

            qint64 written = 0;

            while (written < maxlen)
            {
                auto opt = m_queue->pop();

                if (!opt.has_value())
                {
                    // fill remaining with silence
                    memset(data + written, 0, static_cast<size_t>(maxlen - written));
                    written = maxlen;
                    break;
                }

                auto frame = *opt;

                if (!frame) { continue; }

                auto buf = frame->buffer();

                if (!buf) { continue; }

                size_t toCopy = std::min<size_t>(buf->size(), static_cast<size_t>(maxlen - written));
                memcpy(data + written, buf->data(), toCopy);
                written += static_cast<qint64>(toCopy);

                // If buffer larger than remaining, we drop the tail for now
            }

            return written;
        }

        qint64 writeData(const char* /*data*/, qint64 /*len*/) override { return 0; }

    private:
        std::shared_ptr<camera::core::IAudioFrameQueue> m_queue;
    };

    AudioPlayer::AudioPlayer(QObject* parent)
        : QObject(parent)
        , m_io(nullptr)
        , m_devices(new QMediaDevices(this))
    {
    }

    AudioPlayer::~AudioPlayer()
    {
        stop();
    }

    void AudioPlayer::start(std::shared_ptr<camera::core::IAudioFrameQueue> queue)
    {
        m_queue = std::move(queue);

        QAudioDevice info = m_devices->defaultAudioOutput();

        m_sink.reset(new QAudioSink(info, info.preferredFormat(), this));
        m_io = new AudioIODevice(m_queue, this);
        m_io->open(QIODevice::ReadOnly);
        m_sink->start(m_io);
    }

    void AudioPlayer::stop() noexcept
    {

    }

    void AudioPlayer::setVolume(qreal v)
    {

    }

} // namespace camera::renderer
