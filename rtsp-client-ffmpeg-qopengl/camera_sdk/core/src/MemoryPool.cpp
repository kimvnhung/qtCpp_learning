#include "../include/core/MemoryPool.h"
#include <vector>
#include <new>
#include <cassert>

namespace camera::core {

MemoryPool::MemoryPool(size_t bufferCapacity, size_t initialCount) noexcept
    : m_bufferCapacity(bufferCapacity)
{
    for (size_t i = 0; i < initialCount; ++i) {
        // Preallocate plane layout for a nominal YUV420P frame with offsets 0, ...
        // For simplicity, we allocate a single large buffer; the decoder will
        // arrange plane offsets/strides as needed when using the buffer.
        size_t total = bufferCapacity;
        // Create VideoBuffer with no planes yet; populate with full-size single plane.
        auto vb = new VideoBuffer(total, std::vector<int>{0}, std::vector<int>{static_cast<int>(bufferCapacity)}, std::vector<int>{static_cast<int>(bufferCapacity)});
        pushRaw(vb);
        m_count.fetch_add(1, std::memory_order_relaxed);
    }
}

MemoryPool::~MemoryPool()
{
    // Pop all nodes and delete buffers
    Node* n = m_head.exchange(nullptr);
    while (n) {
        delete n->buf;
        Node* next = n->next;
        delete n;
        n = next;
    }
}

void MemoryPool::pushRaw(VideoBuffer* buf) noexcept
{
    Node* node = new Node{buf, nullptr};
    Node* old = m_head.load(std::memory_order_relaxed);
    do {
        node->next = old;
    } while (!m_head.compare_exchange_weak(old, node, std::memory_order_release, std::memory_order_relaxed));
}

VideoBuffer* MemoryPool::popRaw() noexcept
{
    Node* old = m_head.load(std::memory_order_acquire);
    while (old) {
        Node* next = old->next;
        if (m_head.compare_exchange_weak(old, next, std::memory_order_acq_rel, std::memory_order_acquire)) {
            VideoBuffer* buf = old->buf;
            delete old;
            return buf;
        }
    }
    return nullptr;
}

VideoBufferPtr MemoryPool::acquire()
{
    VideoBuffer* raw = popRaw();
    if (!raw) {
        // allocate new fallback
        raw = new VideoBuffer(m_bufferCapacity, std::vector<int>{0}, std::vector<int>{static_cast<int>(m_bufferCapacity)}, std::vector<int>{static_cast<int>(m_bufferCapacity)});
    } else {
        m_count.fetch_sub(1, std::memory_order_relaxed);
    }

    // Create shared_ptr with custom deleter that returns the buffer to the pool
    MemoryPool* pool = this;
    return VideoBufferPtr(raw, [pool](VideoBuffer* p) {
        // Return to pool
        pool->pushRaw(p);
        pool->m_count.fetch_add(1, std::memory_order_relaxed);
    });
}

size_t MemoryPool::available() const noexcept
{
    return m_count.load(std::memory_order_relaxed);
}

} // namespace camera::core
