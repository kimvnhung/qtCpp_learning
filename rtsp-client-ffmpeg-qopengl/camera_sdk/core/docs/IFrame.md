IFrame
======

Summary
-------
`IFrame` is a small, read-only interface describing a video frame (format, size, plane access, timestamp).

Design notes
------------
- Interface-first design enables different frame implementations (zero-copy GPU-backed frames, CPU-owned buffers).
- All accessors are const and noexcept to allow usage from performance-sensitive code paths.

Thread safety
-------------
- `IFrame` implementations must document their threading guarantees. The provided `VideoFrame` is immutable and thread-safe via shared ownership.
