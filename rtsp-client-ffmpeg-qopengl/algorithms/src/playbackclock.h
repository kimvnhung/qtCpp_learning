#ifndef PLAYBACKCLOCK_H
#define PLAYBACKCLOCK_H

#include <chrono>

class PlaybackClock
{
public:
    PlaybackClock();
public:
    void play();
    void pause();
    void seek(int64_t ms);
    void setRate(double rate);
    void reset();
    bool isPlaying() const;

    int64_t currentTime() const;
private:
    // Time from last play or seek, in milliseconds
    std::chrono::steady_clock::time_point m_baseTime;
    // Time offset from the start of the media, in milliseconds
    int64_t m_offset{0};
    double m_rate = 1.0;
    bool m_paused = true;
};

#endif // PLAYBACKCLOCK_H
