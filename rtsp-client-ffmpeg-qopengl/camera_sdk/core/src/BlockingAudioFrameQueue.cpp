// BlockingAudioFrameQueue.cpp
#include <core/BlockingAudioFrameQueue.h>

namespace camera::core {

bool BlockingAudioFrameQueue::push(std::shared_ptr<AudioFrame> frame)
{
    std::unique_lock lock(mtx);
    q.emplace_back(std::move(frame));
    cv.notify_one();
    return true;
}

std::optional<std::shared_ptr<AudioFrame>> BlockingAudioFrameQueue::pop()
{
    std::unique_lock lock(mtx);

    if (q.empty()) { cv.wait_for(lock, std::chrono::milliseconds(100), [&]() { return !q.empty(); }); }

    if (q.empty()) return std::nullopt;

    auto f = q.front();
    q.pop_front();
    return f;
}

size_t BlockingAudioFrameQueue::size() const noexcept
{
    std::unique_lock lock(mtx);
    return q.size();
}

} // namespace camera::core
