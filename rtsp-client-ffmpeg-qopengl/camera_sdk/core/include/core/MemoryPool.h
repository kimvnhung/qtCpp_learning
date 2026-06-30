// MemoryPool.h
#pragma once

#include "VideoBuffer.h"
#include <atomic>
#include <memory>
#include <cstddef>

namespace camera::core {

// A simple lock-free pool of VideoBuffer instances. The pool returns
// std::shared_ptr<VideoBuffer> where the deleter returns the raw buffer
// back to the pool. The pool must outlive any buffers checked out from it.
class MemoryPool : public std::enable_shared_from_this<MemoryPool> {
public:
    explicit MemoryPool(size_t bufferCapacity, size_t initialCount = 4) noexcept;
    ~MemoryPool();

    // Acquire a buffer from the pool. If pool is empty, a new buffer is allocated.
    VideoBufferPtr acquire();

    // Current available count (approximate)
    size_t available() const noexcept;

private:
    struct Node {
        VideoBuffer* buf;
        Node* next;
    };

    void pushRaw(VideoBuffer* buf) noexcept;
    VideoBuffer* popRaw() noexcept;

    std::atomic<Node*> m_head{nullptr};
    size_t m_bufferCapacity;
    std::atomic_size_t m_count{0};
};

using MemoryPoolPtr = std::shared_ptr<MemoryPool>;

} // namespace camera::core
