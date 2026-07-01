#ifndef IFRAMEQUEUE_H
#define IFRAMEQUEUE_H

#include <deque>
#include <memory>
#include <mutex>
#include <optional>

template <typename T>
class ThreadSafeQueue
{
public:
    ~ThreadSafeQueue() = default;

    // Push a frame into the queue. Ownership of the frame is transferred.
    bool push(std::shared_ptr<T> frame)
    {

    }

    // Pop a frame from the queue. Returns nullopt if empty.
    std::optional<std::shared_ptr<T>> pop() = 0;

    // Current number of frames (may be approximate for lock-free implementations).
    size_t size() const noexcept = 0;
private:
    mutable std::mutex mtx;
    std::deque<std::shared_ptr<T>> q;
};

#endif // IFRAMEQUEUE_H
