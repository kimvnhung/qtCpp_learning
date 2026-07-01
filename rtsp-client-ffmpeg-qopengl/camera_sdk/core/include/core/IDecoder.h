// IDecoder.h
// Abstract decoder interface. Implementations decode encoded packets and
// emit frames to a provided frame queue.
#pragma once

#include "IFrameQueue.h"
#include "IAudioFrameQueue.h"
#include <string>
#include <memory>

namespace camera::core {

struct DecoderConfig {
    std::string url; // input URL (rtsp, file...)
    int maxThreads = 1; // decoder threads, subject to implementation
    std::shared_ptr<IAudioFrameQueue> audioQueue; // optional audio output queue
};

class IDecoder {
public:
    virtual ~IDecoder() = default;

    // Start decoding (async). Frames should be pushed to the queue.
    virtual bool start(const DecoderConfig& cfg, std::shared_ptr<IFrameQueue> outQueue) = 0;

    // Stop decoding and join internal threads. Must be safe to call multiple times.
    virtual void stop() noexcept = 0;
};

} // namespace camera::core
