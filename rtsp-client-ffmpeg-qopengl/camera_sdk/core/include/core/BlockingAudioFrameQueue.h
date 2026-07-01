// BlockingAudioFrameQueue.h
#pragma once

#include "IAudioFrameQueue.h"
#include <deque>
#include <mutex>
#include <condition_variable>

namespace camera::core {

class BlockingAudioFrameQueue : public IAudioFrameQueue
{
public:
    BlockingAudioFrameQueue() = default;

    bool push(std::shared_ptr<AudioFrame> frame) override;
    std::optional<std::shared_ptr<AudioFrame>> pop() override;
    size_t size() const noexcept override;

private:
    mutable std::mutex mtx;
    std::condition_variable cv;
    std::deque<std::shared_ptr<AudioFrame>> q;
};

} // namespace camera::core
