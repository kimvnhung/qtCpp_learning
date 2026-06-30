VideoFrame
==========

Summary
-------
`VideoFrame` is a concrete, reference-counted container for decoded video frame data in YUV formats (YUV420P, NV12).

Thread ownership
----------------
- Instances are immutable after construction and may be safely shared across threads via `std::shared_ptr`.

Lifetime
--------
- Constructed via `VideoFrame::create(...)` which returns a `std::shared_ptr<VideoFrame>`.
- Lifetime is managed by `shared_ptr` owners, typically the producer (decoder) and one or more consumers (renderer/recorder).

Memory ownership
----------------
- Each plane is stored in a `std::shared_ptr<const std::vector<uint8_t>>` to provide stable ownership and allow zero-copy sharing.

Reason of design
----------------
- Shared ownership simplifies passing frames between decoder, queue, and renderer without copying buffers.
- Immutable data ensures thread-safety without locks for read access.

Trade-offs
----------
- Using `shared_ptr<vector>` increases small-object overhead compared to a single contiguous allocation, but simplifies lifetime and avoids custom allocators.
- Stride inference is conservative; downstream components (renderer) should query `stride()` and handle per-format layout.

Important functions
-------------------
- `create(...)`: Validates inputs and returns a shared_ptr-managed instance.
- `planeData(int)`: Returns a read-only `std::span` view over plane bytes.
