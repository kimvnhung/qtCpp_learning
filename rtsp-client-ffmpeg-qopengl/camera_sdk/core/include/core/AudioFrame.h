// AudioFrame.h
#pragma once

#include "AudioBuffer.h"
#include "IFrame.h"
#include <memory>
#include <chrono>

namespace camera::core {

struct AudioFrameMetadata
{
    IFrame::Timestamp pts{};
    std::chrono::microseconds duration{0};
};

class AudioFrame
{
public:
    AudioFrame() = default;

    static std::shared_ptr<AudioFrame> create(AudioBufferPtr buf, const AudioFrameMetadata& meta)
    {
        auto p = std::make_shared<AudioFrame>();
        p->m_buf = std::move(buf);
        p->m_meta = meta;
        return p;
    }

    AudioBufferPtr buffer() const noexcept { return m_buf; }
    const AudioFrameMetadata& metadata() const noexcept { return m_meta; }

private:
    AudioBufferPtr m_buf;
    AudioFrameMetadata m_meta;
};

using AudioFramePtr = std::shared_ptr<AudioFrame>;

} // namespace camera::core
