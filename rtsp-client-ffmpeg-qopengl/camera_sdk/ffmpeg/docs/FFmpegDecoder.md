FFmpeg Decoder internals
========================

This document explains the key FFmpeg API calls and how the decoder module uses them.

avformat_open_input
--------------------
- Opens an input (file, RTSP URL, etc.) and allocates an `AVFormatContext`.
- It performs protocol-level initialization and prepares the format context for `avformat_find_stream_info` and reading packets.
- In our code we call this to both probe stream info and later to open the actual decoding context. Errors should be logged and, for network sources, retried with backoff.

avcodec_send_packet
-------------------
- Queues an `AVPacket` into the decoder. It returns 0 on success, `AVERROR(EAGAIN)` if the decoder needs to be drained (`avcodec_receive_frame` must be called first), or other negative error codes.
- The correct pattern: send packet(s) with `avcodec_send_packet`, then call `avcodec_receive_frame` repeatedly to retrieve decoded frames until `AVERROR(EAGAIN)`.

avcodec_receive_frame
---------------------
- Pops a decoded `AVFrame` from the decoder. It returns 0 on success, `AVERROR(EAGAIN)` if more packets are needed, or `AVERROR_EOF` if the decoder has been flushed/ended.
- Always unref/free `AVFrame` after use.

PTS / DTS / TimeBase
---------------------
- `AVFrame->pts` is the presentation timestamp (when this frame should be shown).
- `AVPacket->dts` or `AVFrame->pkt_dts` may contain decode timestamps.
- Timestamps are expressed in the stream's `time_base` units (an `AVRational`). Convert to seconds: `pts * (time_base.num / time_base.den)`.
- Our implementation converts timestamps to microseconds using `av_rescale_q` and stores them in `FrameMetadata` as `std::chrono::microseconds` wrapped in a `steady_clock::time_point` for downstream use.

Error recovery and reconnect
---------------------------
- For network sources (RTSP), transient network errors are common. The `PacketReader` reads frames in a loop; on error it closes the input and retries after a short delay.
- An interrupt callback may be provided to the format context to allow cancellation and timeouts. We implement a simple timeout by tracking last activity and returning non-zero from the interrupt callback when the timeout is exceeded.
- On fatal codec errors, the decoder thread logs and attempts to drop problematic packets and continue where possible. For unrecoverable errors the module will stop and report failure.

Flush / Seek
------------
- To flush the decoder (for seeking or end-of-stream), call `avcodec_send_packet(codec, nullptr)`. Then call `avcodec_receive_frame` until it returns `AVERROR_EOF` or `AVERROR(EAGAIN)` to drain pending frames.
- To seek, call `av_seek_frame` on the `AVFormatContext` and then `avcodec_flush_buffers` on the `AVCodecContext`. The packet queue must be flushed to remove stale packets.

Threading model
---------------
- `PacketReader` runs in one thread performing `av_read_frame` and pushing `AVPacket`s to a `PacketQueue`.
- `DecoderWorker` runs in another thread popping packets, calling `avcodec_send_packet`/`avcodec_receive_frame`, converting `AVFrame`s to `VideoFrame`s and pushing them to the `IFrameQueue`.
