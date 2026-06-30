#include "../include/core/VideoFrame.h"

namespace camera::core {

std::shared_ptr<VideoFrame> VideoFrame::create(VideoBufferPtr buffer, FrameMetadata meta) noexcept
{
    if (!buffer) return nullptr;
    if (meta.width <= 0 || meta.height <= 0) return nullptr;

    // Validate plane count for supported formats
    if ((meta.format == PixelFormat::YUV420P && buffer->planes() < 3) ||
        (meta.format == PixelFormat::NV12 && buffer->planes() < 2)) {
        return nullptr;
    }

    return std::shared_ptr<VideoFrame>(new VideoFrame(std::move(buffer), meta));
}

VideoFrame::VideoFrame(VideoBufferPtr buffer, FrameMetadata meta) noexcept
    : m_buffer(std::move(buffer))
    , m_meta(std::move(meta))
{
}

std::span<const uint8_t> VideoFrame::planeData(int plane) const noexcept
{
    if (!m_buffer) return {};
    return m_buffer->planeData(plane);
}

int VideoFrame::stride(int plane) const noexcept
{
    if (!m_buffer) return 0;
    return m_buffer->stride(plane);
}

} // namespace camera::core
