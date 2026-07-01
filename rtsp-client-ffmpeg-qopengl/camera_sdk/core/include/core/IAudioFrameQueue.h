// IAudioFrameQueue.h
#pragma once

#include "AudioFrame.h"
#include <memory>
#include <optional>

namespace camera::core {

class IAudioFrameQueue
{
public:
    virtual ~IAudioFrameQueue() = default;

    virtual bool push(std::shared_ptr<AudioFrame> frame) = 0;
    virtual std::optional<std::shared_ptr<AudioFrame>> pop() = 0;
    virtual size_t size() const noexcept = 0;
};

} // namespace camera::core
