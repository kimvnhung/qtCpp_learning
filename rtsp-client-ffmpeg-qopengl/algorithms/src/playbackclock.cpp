#include "playbackclock.h"

#include "log.h"

PlaybackClock::PlaybackClock()
    : m_baseTime(std::chrono::steady_clock::now())
    , m_offset(0)
    , m_rate(1.0)
    , m_paused(true)
{

}

void PlaybackClock::play()
{
    if (m_paused)
    {
        m_baseTime = std::chrono::steady_clock::now();
        m_paused = false;
    }
    else
    {
        LOGW() << "PlaybackClock is already playing.";
    }
}

void PlaybackClock::pause()
{
    if (!m_paused)
    {
        m_offset += std::chrono::duration<double>(std::chrono::steady_clock::now() - m_baseTime).count() * 1000 * m_rate;
        m_paused = true;
    }
    else
    {
        LOGW() << "PlaybackClock is already paused.";
    }
}

void PlaybackClock::seek(int64_t ms)
{
    m_offset = ms;
    m_baseTime = std::chrono::steady_clock::now();
}

void PlaybackClock::setRate(double rate)
{
    if (rate <= 0.0)
    {
        LOGW() << "PlaybackClock: Rate must be positive.";
        return;
    }

    if (!m_paused)
    {
        m_offset += std::chrono::duration<double>(std::chrono::steady_clock::now() - m_baseTime).count() * 1000 * m_rate;
        m_baseTime = std::chrono::steady_clock::now();
    }

    m_rate = rate;
}

void PlaybackClock::reset()
{
    m_offset = 0;
    m_baseTime = std::chrono::steady_clock::now();
    m_rate = 1.0;
    m_paused = true;
}

bool PlaybackClock::isPlaying() const
{
    return !m_paused;
}

int64_t PlaybackClock::currentTime() const
{
    if (m_paused)
    {
        return m_offset;
    }
    else
    {
        return m_offset + std::chrono::duration<double>(std::chrono::steady_clock::now() - m_baseTime).count() * 1000 * m_rate;
    }
}
