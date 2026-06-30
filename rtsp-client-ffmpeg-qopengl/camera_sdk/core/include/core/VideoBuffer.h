// VideoBuffer.h
#pragma once

#include <cstdint>
#include <vector>
#include <memory>
#include <span>

namespace camera::core {

// Describes the color range and colorspace of a frame.
enum class ColorSpace {
    Unknown,
    BT601,
    BT709,
    BT2020
};

enum class ColorRange {
    Unknown,
    Limited,
    Full
};

// VideoBuffer owns contiguous memory for one decoded frame's planes and
// provides per-plane offsets and stride information. It is intended to be
// cheaply shared via std::shared_ptr.
class VideoBuffer {
public:
    VideoBuffer() = delete;
    VideoBuffer(size_t totalSize,
                std::vector<int> planeOffsets,
                std::vector<int> planeStrides,
                std::vector<int> planeSizes) noexcept;

    // Access raw mutable plane bytes.
    std::span<uint8_t> planeDataMutable(int plane) noexcept;
    // Access raw const plane bytes.
    std::span<const uint8_t> planeData(int plane) const noexcept;

    int stride(int plane) const noexcept;
    int planeSize(int plane) const noexcept;
    int planes() const noexcept { return static_cast<int>(m_planeOffsets.size()); }

    size_t capacity() const noexcept { return m_data.size(); }

private:
    std::vector<uint8_t> m_data;
    std::vector<int> m_planeOffsets;
    std::vector<int> m_planeStrides;
    std::vector<int> m_planeSizes;
};

using VideoBufferPtr = std::shared_ptr<VideoBuffer>;

} // namespace camera::core
