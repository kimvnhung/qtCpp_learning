// AudioBuffer.h
#pragma once

#include <cstddef>
#include <memory>
#include <vector>

namespace camera::core {

enum class SampleFormat {
    UNKNOWN = 0,
    S16,
    FLT,
};

class AudioBuffer
{
public:
    AudioBuffer() = default;

    AudioBuffer(size_t bytes, int sampleRate, int channels, int nbSamples, SampleFormat fmt)
        : m_data(bytes), m_sampleRate(sampleRate), m_channels(channels), m_nbSamples(nbSamples), m_format(fmt)
    {}

    static std::shared_ptr<AudioBuffer> create(size_t bytes, int sampleRate, int channels, int nbSamples, SampleFormat fmt)
    {
        return std::make_shared<AudioBuffer>(bytes, sampleRate, channels, nbSamples, fmt);
    }

    uint8_t* data() noexcept { return m_data.data(); }
    const uint8_t* data() const noexcept { return m_data.data(); }
    size_t size() const noexcept { return m_data.size(); }

    int sampleRate() const noexcept { return m_sampleRate; }
    int channels() const noexcept { return m_channels; }
    int nbSamples() const noexcept { return m_nbSamples; }
    SampleFormat format() const noexcept { return m_format; }

private:
    std::vector<uint8_t> m_data;
    int m_sampleRate = 0;
    int m_channels = 0;
    int m_nbSamples = 0;
    SampleFormat m_format = SampleFormat::UNKNOWN;
};

using AudioBufferPtr = std::shared_ptr<AudioBuffer>;

} // namespace camera::core
