#include "../include/core/VideoBuffer.h"
#include <algorithm>

namespace camera::core {

VideoBuffer::VideoBuffer(size_t totalSize,
                         std::vector<int> planeOffsets,
                         std::vector<int> planeStrides,
                         std::vector<int> planeSizes) noexcept
    : m_data(totalSize)
    , m_planeOffsets(std::move(planeOffsets))
    , m_planeStrides(std::move(planeStrides))
    , m_planeSizes(std::move(planeSizes))
{
}

std::span<uint8_t> VideoBuffer::planeDataMutable(int plane) noexcept
{
    if (plane < 0 || static_cast<size_t>(plane) >= m_planeOffsets.size()) return {};
    auto off = static_cast<size_t>(m_planeOffsets[plane]);
    return std::span<uint8_t>(m_data.data() + off, static_cast<size_t>(m_planeSizes[plane]));
}

std::span<const uint8_t> VideoBuffer::planeData(int plane) const noexcept
{
    if (plane < 0 || static_cast<size_t>(plane) >= m_planeOffsets.size()) return {};
    auto off = static_cast<size_t>(m_planeOffsets[plane]);
    return std::span<const uint8_t>(m_data.data() + off, static_cast<size_t>(m_planeSizes[plane]));
}

int VideoBuffer::stride(int plane) const noexcept
{
    if (plane < 0 || static_cast<size_t>(plane) >= m_planeStrides.size()) return 0;
    return m_planeStrides[plane];
}

int VideoBuffer::planeSize(int plane) const noexcept
{
    if (plane < 0 || static_cast<size_t>(plane) >= m_planeSizes.size()) return 0;
    return m_planeSizes[plane];
}

} // namespace camera::core
