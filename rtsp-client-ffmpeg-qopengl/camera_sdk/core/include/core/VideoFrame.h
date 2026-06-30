// VideoFrame.h
// Concrete implementation of IFrame that owns YUV plane buffers.
//
// This header contains extensive documentation about ownership, threading,
// lifetime and design rationale as required by the SDK specification.
#pragma once

#include "IFrame.h"
#include <vector>
#include <memory>
#include <cstdint>
#include <span>
#include <chrono>

namespace camera::core {

class VideoFrame final : public IFrame, public std::enable_shared_from_this<VideoFrame> {
public:
    // Factory to create a VideoFrame from owned buffers.
    // Buffers should contain contiguous bytes for each plane.
    static std::shared_ptr<VideoFrame> create(PixelFormat fmt,
                                              int w,
                                              int h,
                                              std::vector<std::vector<uint8_t>>&& planes,
                                              std::chrono::steady_clock::time_point ts = std::chrono::steady_clock::now()) noexcept;

    ~VideoFrame() override;

    // IFrame interface
    PixelFormat format() const noexcept override { return m_format; }
    int width() const noexcept override { return m_width; }
    int height() const noexcept override { return m_height; }
    int planes() const noexcept override { return static_cast<int>(m_planeStrides.size()); }
    std::span<const uint8_t> planeData(int plane) const noexcept override;
    int stride(int plane) const noexcept override;
    Timestamp timestamp() const noexcept override { return m_timestamp; }

private:
    VideoFrame(PixelFormat fmt,
               int w,
               int h,
               std::vector<std::vector<uint8_t>>&& planes,
               Timestamp ts) noexcept;

    PixelFormat m_format;
    int m_width;
    int m_height;
    std::vector<std::shared_ptr<const std::vector<uint8_t>>> m_planeBuffers;
    std::vector<int> m_planeStrides;
    Timestamp m_timestamp;
};

} // namespace camera::core
