#include "MediaPlayer.h"

#include <ffmpeg/FFmpegDecoder.h>
#include <core/TripleBufferFrameQueue.h>
#include <core/BlockingAudioFrameQueue.h>
#include <renderer/AudioPlayer.h>
#include <core/IFrame.h>
#include <renderer/VideoItem.h>

#include <chrono>
#include <QMetaObject>

using namespace camera::ffmpeg;
using namespace camera::core;
using namespace camera::renderer;

MediaPlayer::MediaPlayer(QObject* parent)
    : QObject(parent)
    , m_decoder(std::make_unique<FFmpegDecoder>())
    , m_queue(std::make_shared<TripleBufferFrameQueue>())
{
    m_audioQueue = std::make_shared<BlockingAudioFrameQueue>();
    m_audioPlayer = std::make_unique<AudioPlayer>();
}

MediaPlayer::~MediaPlayer()
{
    stop();
}

void MediaPlayer::start(const QString& url)
{
    if (m_running.load()) return;

    m_url = url;
    m_running.store(true);

    DecoderConfig cfg{};
    cfg.url = m_url.toStdString();
    cfg.audioQueue = m_audioQueue;

    m_decoder->start(cfg, m_queue);
    m_audioPlayer->start(m_audioQueue);

    m_thread = std::thread([this]() { pumpLoop(); });
}

void MediaPlayer::stop()
{
    if (!m_running.load()) return;

    m_running.store(false);

    if (m_thread.joinable()) m_thread.join();

    if (m_decoder) m_decoder->stop();

    if (m_audioPlayer) m_audioPlayer->stop();
}

void MediaPlayer::addVideoItem(QObject* item)
{
    if (!item) return;

    std::lock_guard<std::mutex> lk(m_mutex);
    m_items.push_back(item);
}

void MediaPlayer::setVolume(qreal v)
{
    if (m_audioPlayer) m_audioPlayer->setVolume(v);
}

void MediaPlayer::pumpLoop()
{
    using clock = std::chrono::steady_clock;
    auto lastTime = clock::now();
    int frameCount = 0;

    while (m_running.load())
    {
        auto opt = m_queue->latestFrame();

        if (opt)
        {
            auto frame = *opt;

            if (frame)
            {
                m_width = frame->width();
                m_height = frame->height();
            }

            std::lock_guard<std::mutex> lk(m_mutex);

            for (QObject * obj : m_items)
            {
                auto vi = qobject_cast<VideoItem *>(obj);

                if (vi)
                {
                    std::shared_ptr<IFrame> f = frame;
                    QMetaObject::invokeMethod(vi, [vi, f]() { vi->present(f); }, Qt::QueuedConnection);
                }
            }

            frameCount++;
            auto now = clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTime).count();

            if (elapsed >= 1000)
            {
                m_fps = frameCount * 1000 / static_cast<int>(elapsed);
                emit infoChanged();
                frameCount = 0;
                lastTime = now;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}
