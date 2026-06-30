// Minimal FFmpegDecoder skeleton. Actual decoding will be implemented in later parts.
#include "../include/ffmpeg/FFmpegDecoder.h"
#include <atomic>

namespace camera::ffmpeg {

FFmpegDecoder::FFmpegDecoder() = default;

FFmpegDecoder::~FFmpegDecoder() { stop(); }

bool FFmpegDecoder::start(const camera::core::DecoderConfig& cfg, std::shared_ptr<camera::core::IFrameQueue> outQueue)
{
    // This skeleton doesn't implement decoding yet. It validates parameters and returns true.
    (void)cfg; (void)outQueue;
    return true;
}

void FFmpegDecoder::stop() noexcept
{
    // Stop background operations if any.
}

} // namespace camera::ffmpeg
