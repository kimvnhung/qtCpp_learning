VideoFrame design
=================

Summary
-------
This document explains the `VideoFrame`, `VideoBuffer`, and `MemoryPool`
design decisions used in the Camera SDK core module.

Key types
---------
- `VideoBuffer`: owns contiguous memory for a frame and stores per-plane offsets and strides.
- `VideoFrame`: lightweight metadata wrapper around a `VideoBuffer` that exposes `IFrame`.
- `MemoryPool`: lock-free freelist of `VideoBuffer` objects for reuse and reduced allocations.

Why `VideoFrame` never exposes `AVFrame`
----------------------------------------
- Portability: `AVFrame` is part of FFmpeg's ABI and would leak FFmpeg types into all consumers. Keeping FFmpeg internal allows the SDK to be swapped or compiled without FFmpeg for components that don't need it.
- Dependency isolation: renderers and platform integrations (Qt Quick/QRhi) should not depend on codec/format libraries. This keeps renderer code portable and smaller (no link-time dependency on FFmpeg) and simplifies cross-compilation (Android, Windows).
- Ownership clarity: `AVFrame` contains reference-counted buffers and complicated lifetime rules. By copying necessary metadata into a `VideoBuffer` and `VideoFrame`, we provide a simpler, well-documented ownership model (RAII via `std::shared_ptr`).

Why the renderer must not depend on FFmpeg
-----------------------------------------
- Separation of concerns: the renderer's job is to upload YUV plane data to GPU textures and draw them using QRhi. It should not need to know about decoding internals.
- Build & binary size: FFmpeg is large and introduces many transitive dependencies. Keeping renderer independent allows building renderer into UI libraries without pulling FFmpeg everywhere.
- Flexibility for hardware acceleration: decoders may provide GPU-backed frames (DMA-BUF, CVPixelBuffer, etc.). The renderer only needs a `VideoBuffer` abstraction (or platform-specific extensions) to consume such frames without FFmpeg.

Memory ownership & reference counting
------------------------------------
- `VideoBuffer` is owned by `std::shared_ptr<VideoBuffer>` (`VideoBufferPtr`). Multiple `VideoFrame` instances or a queue and a renderer can hold shared_ptrs to the same buffer without copying.
- `MemoryPool` hands out `VideoBufferPtr` instances with a custom deleter that returns the raw buffer to the pool. The pool must outlive any buffers checked out from it.

Trade-offs
----------
- Using `std::shared_ptr` simplifies lifetime but has some atomic refcount cost; if profiling shows overhead, we can replace the shared pointer with custom intrusive refcount or pool-managed handles.
- The current `MemoryPool` uses a lock-free singly-linked list of raw buffer nodes. It assumes `MemoryPool` is long-lived; destroying the pool while buffers exist is undefined. This simplifies the implementation and suits typical usage where the pool is owned by a long-lived manager.
