// TripleBufferFrameQueue.h
#pragma once

#include "IFrameQueue.h"
#include <array>
#include <atomic>
#include <optional>
#include <memory>

namespace camera::core {

// Lock-free triple buffer queue optimized for single-producer single-consumer
// workloads. It provides `push()`, `pop()`, and `latestFrame()` operations.
//
// Characteristics:
// - Triple-buffer: three slots rotated between producer/consumer/spare.
// - Atomic index swaps, no mutex during push/pop.
// - Frame dropping: if producer outpaces consumer, intermediate frames are dropped
//   and only the latest is preserved.
// - Single-producer, single-consumer semantics.
class TripleBufferFrameQueue : public IFrameQueue {
public:
    TripleBufferFrameQueue() noexcept;
    ~TripleBufferFrameQueue() override = default;

    bool push(std::shared_ptr<IFrame> frame) override;
    std::optional<std::shared_ptr<IFrame>> pop() override;
    size_t size() const noexcept override;

    // Return the latest available frame without advancing the consumer.
    // May return nullopt if no new frame is available.
    std::optional<std::shared_ptr<IFrame>> latestFrame() const noexcept;

private:
    // Non-atomic array of slots. Access pattern ensures safety without per-slot atomics:
    // - Only producer writes to the producer-owned slot.
    // - Only consumer reads from the consumer-owned slot.
    std::array<std::shared_ptr<IFrame>, 3> m_slots;

    // Indexes meaning:
    // m_consumerIndex: index currently owned by consumer (non-atomic, consumer-only)
    // m_writeIndex: index currently owned by producer for writing (non-atomic, producer-only)
    // m_latest: atomic index published by producer as the latest available frame
    mutable std::atomic<int> m_latest;
    int m_writeIndex;
    int m_consumerIndex;
};

} // namespace camera::core
