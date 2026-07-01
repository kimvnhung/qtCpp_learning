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
    void seek(double seconds);
    void setRate(double rate);

    double currentTime() const;
private:
    std::chrono::steady_clock::time_point m_startTime;
    double m_mediaStart = 0.0;
    double m_rate = 1.0;
    bool m_paused = true;
};

#endif // PLAYBACKCLOCK_H
