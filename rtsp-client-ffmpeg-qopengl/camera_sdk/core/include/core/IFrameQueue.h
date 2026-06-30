// IFrameQueue.h
// Interface for a thread-safe frame queue used to hand frames between
// producer (decoder) and consumer (renderer/recorder).
#pragma once

#include "IFrame.h"
#include <memory>
#include <optional>

namespace camera::core {

class IFrameQueue {
public:
    virtual ~IFrameQueue() = default;

    // Push a frame into the queue. Ownership of the frame is transferred.
    virtual bool push(std::shared_ptr<IFrame> frame) = 0;

    // Pop a frame from the queue. Returns nullopt if empty.
    virtual std::optional<std::shared_ptr<IFrame>> pop() = 0;

    // Current number of frames (may be approximate for lock-free implementations).
    virtual size_t size() const noexcept = 0;
};

} // namespace camera::core
