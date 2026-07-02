#ifndef IFRAMEQUEUE_H
#define IFRAMEQUEUE_H

#include <condition_variable>
#include <deque>
#include <memory>
#include <mutex>
#include <optional>

class VideoFrame;
class AudioFrame;

template <typename T>
class ThreadSafeQueue
{
public:
    ~ThreadSafeQueue() = default;

    // Push a frame into the queue. Ownership of the frame is transferred.
    bool push(std::shared_ptr<T> frame)
    {
        std::unique_lock lock(mtx);
        q.emplace_back(std::move(frame));
        cv.notify_one();
        return true;
    }

    // Pop a frame from the queue. Returns nullopt if empty.
    std::optional<std::shared_ptr<T>> pop()
    {
        std::unique_lock lock(mtx);

        if (q.empty()) { cv.wait_for(lock, std::chrono::milliseconds(100), [&]() { return !q.empty(); }); }

        if (q.empty()) { return std::nullopt; }

        auto f = q.front();
        q.pop_front();
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
};

class VideoFrameQueue : public ThreadSafeQueue<VideoFrame>
{

};

class AudioFrameQueue : public ThreadSafeQueue<AudioFrame>
{

};

#endif // IFRAMEQUEUE_H
