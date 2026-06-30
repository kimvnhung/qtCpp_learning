// FFmpegDecoder.h
#pragma once

#include <memory>
#include <chrono>
#include <core/IDecoder.h>

namespace camera::ffmpeg {

class FFmpegDecoder : public camera::core::IDecoder {
public:
    FFmpegDecoder();
    ~FFmpegDecoder() override;

    bool start(const camera::core::DecoderConfig& cfg, std::shared_ptr<camera::core::IFrameQueue> outQueue) override;
    void stop() noexcept override;
};

} // namespace camera::ffmpeg
