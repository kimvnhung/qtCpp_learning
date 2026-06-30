#pragma once

#include "IFrame.h"
#include "VideoBuffer.h"
#include <memory>
#include <chrono>
#include <optional>

namespace camera::core {

struct FrameMetadata {
    int width = 0;
    int height = 0;
    PixelFormat format = PixelFormat::YUV420P;
    std::chrono::steady_clock::time_point pts{};
    std::chrono::steady_clock::time_point dts{};
    std::chrono::microseconds duration{0};
    int rotation = 0; // degrees: 0,90,180,270
    ColorSpace colorSpace = ColorSpace::Unknown;
    ColorRange colorRange = ColorRange::Unknown;
};

class VideoFrame final : public IFrame, public std::enable_shared_from_this<VideoFrame> {
public:
    // Create a VideoFrame referencing an existing VideoBuffer. The buffer
    // provides plane storage while VideoFrame carries metadata.
    static std::shared_ptr<VideoFrame> create(VideoBufferPtr buffer, FrameMetadata meta) noexcept;

    ~VideoFrame() override = default;

    // IFrame interface
    PixelFormat format() const noexcept override { return m_meta.format; }
    int width() const noexcept override { return m_meta.width; }
    int height() const noexcept override { return m_meta.height; }
    int planes() const noexcept override { return m_buffer ? m_buffer->planes() : 0; }
    std::span<const uint8_t> planeData(int plane) const noexcept override;
    int stride(int plane) const noexcept override;
    Timestamp timestamp() const noexcept override { return m_meta.pts; }

    // Additional accessors
    std::chrono::steady_clock::time_point dts() const noexcept { return m_meta.dts; }
    std::chrono::microseconds duration() const noexcept { return m_meta.duration; }
    int rotation() const noexcept { return m_meta.rotation; }
    ColorSpace colorSpace() const noexcept { return m_meta.colorSpace; }
    ColorRange colorRange() const noexcept { return m_meta.colorRange; }

    // Expose underlying buffer for zero-copy operations
    VideoBufferPtr buffer() const noexcept { return m_buffer; }

private:
    VideoFrame(VideoBufferPtr buffer, FrameMetadata meta) noexcept;

    VideoBufferPtr m_buffer;
    FrameMetadata m_meta;
};

} // namespace camera::core
