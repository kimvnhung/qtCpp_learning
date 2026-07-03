#ifndef IFRAMEQUEUE_H
#define IFRAMEQUEUE_H

#include <condition_variable>
#include <deque>
#include <memory>
#include <mutex>
#include <optional>

#include "log.h"

class VideoFrame;
class AudioFrame;

template <typename T>
class ThreadSafeQueue
{
public:
    ThreadSafeQueue() = default;
    ThreadSafeQueue(int maxSize) : maxSize(maxSize) {}
    ~ThreadSafeQueue() = default;

    // Push a frame into the queue. Ownership of the frame is transferred.
    bool push(std::shared_ptr<T> frame)
    {
        std::unique_lock lock(mtx);

        if (q.size() >= maxSize)
        {
            LOGD() << "Queue is full. Waiting for space to push frame.";
            cv.wait(lock, [&]() { return q.size() < maxSize; });
        }

        q.emplace_back(std::move(frame));
        cv.notify_one();
        return true;
    }

    // Get the top item but dont remove it. Returns nullopt if empty.
    std::optional<std::shared_ptr<T>> peek()
    {
        std::unique_lock lock(mtx);

        if (q.empty())
        {
            LOGD() << "Queue is empty. Waiting for frames to peek.";
            cv.wait_for(lock, std::chrono::milliseconds(100), [&]() { return !q.empty(); });
        }

        if (q.empty())
        {
            LOGD() << "Queue is still empty after waiting. Returning nullopt.";
            return std::nullopt;
        }

        return q.front();
    }

    // Pop a frame from the queue. Returns nullopt if empty.
    std::optional<std::shared_ptr<T>> pop()
    {
        std::unique_lock lock(mtx);

        if (q.empty())
        {
            LOGD() << "Queue is empty. Waiting for frames to pop.";
            cv.wait_for(lock, std::chrono::milliseconds(100), [&]() { return !q.empty(); });
        }

        if (q.empty())
        {
            LOGD() << "Queue is still empty after waiting. Returning nullopt.";
            return std::nullopt;
        }

        auto f = q.front();
        q.pop_front();
        cv.notify_one();
        return f;
    }

    // Current number of frames (may be approximate for lock-free implementations).
    size_t size() const noexcept
    {
        std::unique_lock lock(mtx);
        return q.size();
    }
private:
    mutable std::mutex mtx;
    std::condition_variable cv;
    std::deque<std::shared_ptr<T>> q;
    int maxSize{100}; // Maximum number of frames in the queue
};

class VideoFrameQueue : public ThreadSafeQueue<VideoFrame>
{
public:
    VideoFrameQueue(int maxSize = 100) : ThreadSafeQueue<VideoFrame>(maxSize) {}
};

class AudioFrameQueue : public ThreadSafeQueue<AudioFrame>
{
public:
    AudioFrameQueue(int maxSize = 100) : ThreadSafeQueue<AudioFrame>(maxSize) {}
};

#endif // IFRAMEQUEUE_H
