#include "../include/core/TripleBufferFrameQueue.h"

namespace camera::core {

TripleBufferFrameQueue::TripleBufferFrameQueue() noexcept
    : m_latest(2)
    , m_writeIndex(1)
    , m_consumerIndex(0)
{
}

bool TripleBufferFrameQueue::push(std::shared_ptr<IFrame> frame)
{
    // Producer writes into the write-index slot without locking.
    m_slots[m_writeIndex] = std::move(frame);

    // Publish the newly written slot as the latest by atomically swapping
    // the write index with the latest index. The returned previous latest
    // becomes the new write index (spare).
    int prev = m_latest.exchange(m_writeIndex, std::memory_order_acq_rel);
    m_writeIndex = prev;
    return true;
}

std::optional<std::shared_ptr<IFrame>> TripleBufferFrameQueue::pop()
{
    // Atomically swap the latest index with the consumer index. The return
    // value is the most recent slot index; if it's equal to the consumer
    // index, there is no new frame.
    int latest = m_latest.exchange(m_consumerIndex, std::memory_order_acq_rel);
    if (latest == m_consumerIndex) return std::nullopt; // no new frame

    // Move shared_ptr out of the slot to return it to the consumer.
    auto frame = std::move(m_slots[latest]);

    // Clear the slot to release memory earlier. The slot will be reused by producer.
    m_slots[latest].reset();

    // Advance consumer index to the slot we just consumed.
    m_consumerIndex = latest;
    return frame;
}

size_t TripleBufferFrameQueue::size() const noexcept
{
    int latest = m_latest.load(std::memory_order_acquire);
    return (latest == m_consumerIndex) ? 0u : 1u;
}

std::optional<std::shared_ptr<IFrame>> TripleBufferFrameQueue::latestFrame() const noexcept
{
    int latest = m_latest.load(std::memory_order_acquire);
    if (latest == m_consumerIndex) return std::nullopt;
    // Return a copy of the shared_ptr. Copying updates refcount atomically.
    return m_slots[latest];
}

} // namespace camera::core
