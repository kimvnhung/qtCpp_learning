// IFrame.h
// Interface representing an immutable video frame exposing plane data.
//
// Detailed explanation, ownership, and threading notes are in accompanying
// docs/ files for the concrete implementations.
#pragma once

#include <cstdint>
#include <cstddef>
#include <memory>
#include <span>
#include <chrono>

namespace camera::core {

enum class PixelFormat {
    YUV420P,
    NV12,
};

class IFrame {
public:
    using Timestamp = std::chrono::steady_clock::time_point;

    virtual ~IFrame() = default;

    // Pixel format of the frame.
    virtual PixelFormat format() const noexcept = 0;

    // Width and height of the luma plane.
    virtual int width() const noexcept = 0;
    virtual int height() const noexcept = 0;

    // Number of planes (2 for NV12, 3 for YUV420P).
    virtual int planes() const noexcept = 0;

    // Access raw plane data as a read-only span.
    virtual std::span<const uint8_t> planeData(int plane) const noexcept = 0;

    // Stride (bytes per row) for given plane.
    virtual int stride(int plane) const noexcept = 0;

    // Presentation timestamp of the frame.
    virtual Timestamp timestamp() const noexcept = 0;
};

} // namespace camera::core
