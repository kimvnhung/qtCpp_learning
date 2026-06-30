// FFmpegDecoder.h
#pragma once

#include <memory>
#include <core/IDecoder.h>

namespace camera::ffmpeg {

class FFmpegDecoderImpl;


class FFmpegDecoder : public camera::core::IDecoder {
public:
    FFmpegDecoder();
    ~FFmpegDecoder() override;

    bool start(const camera::core::DecoderConfig& cfg, std::shared_ptr<camera::core::IFrameQueue> outQueue) override;
    void stop() noexcept override;

private:
    std::unique_ptr<FFmpegDecoderImpl> impl;
};

} // namespace camera::ffmpeg
