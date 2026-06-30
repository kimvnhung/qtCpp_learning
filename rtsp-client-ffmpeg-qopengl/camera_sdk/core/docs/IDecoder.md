IDecoder
========

Summary
-------
`IDecoder` defines the lifecycle of a decoder implementation: `start()` begins asynchronous decoding into an `IFrameQueue`, `stop()` halts decoding and joins internal threads.

Design notes
------------
- Decoders should never block callers on long operations; `start()` should spawn threads or tasks.
- `stop()` must be noexcept and idempotent.
