#include "../include/core/VideoFrame.h"
#include <utility>

namespace camera::core {

std::shared_ptr<VideoFrame> VideoFrame::create(PixelFormat fmt,
                                              int w,
                                              int h,
                                              std::vector<std::vector<uint8_t>>&& planes,
                                              std::chrono::steady_clock::time_point ts) noexcept
{
    // Validate minimal invariants for supported formats.
    if (w <= 0 || h <= 0) return nullptr;

    // For YUV420P expect 3 planes, for NV12 expect 2 planes.
    if ((fmt == PixelFormat::YUV420P && planes.size() != 3) ||
        (fmt == PixelFormat::NV12 && planes.size() != 2)) {
        return nullptr;
    }

    return std::shared_ptr<VideoFrame>(new VideoFrame(fmt, w, h, std::move(planes), ts));
}

VideoFrame::VideoFrame(PixelFormat fmt,
                       int w,
                       int h,
                       std::vector<std::vector<uint8_t>>&& planes,
                       Timestamp ts) noexcept
    : m_format(fmt)
    , m_width(w)
    , m_height(h)
    , m_timestamp(ts)
{
    m_planeBuffers.reserve(planes.size());
    m_planeStrides.reserve(planes.size());
    // Move each plane into a shared_ptr<vector<uint8_t>> to provide stable
    // ownership and allow cheap sharing with renderers/queues.
    for (auto &p : planes) {
        auto buf = std::make_shared<const std::vector<uint8_t>>(std::move(p));
        m_planeStrides.push_back(static_cast<int>(buf->size() / std::max(1, h))); // conservative stride
        m_planeBuffers.push_back(buf);
    }
}

VideoFrame::~VideoFrame() = default;

std::span<const uint8_t> VideoFrame::planeData(int plane) const noexcept
{
    if (plane < 0 || static_cast<size_t>(plane) >= m_planeBuffers.size()) return {};
    const auto &buf = *m_planeBuffers[plane];
    return std::span<const uint8_t>(buf.data(), buf.size());
}

int VideoFrame::stride(int plane) const noexcept
{
    if (plane < 0 || static_cast<size_t>(plane) >= m_planeStrides.size()) return 0;
    return m_planeStrides[plane];
}

} // namespace camera::core
