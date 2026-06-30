Triple Buffer Frame Queue
=========================

Overview
--------
This module implements a lock-free triple-buffered queue optimized for a
single-producer, single-consumer (SPSC) scenario such as Decoder -> Renderer.

How it works
------------
- There are exactly three slots (buffers). At any time one slot is owned by the
  consumer (m_consumerIndex), one slot is owned by the producer for writing
  (m_writeIndex), and one slot holds the latest published frame (m_latest).
- The producer writes into `m_slots[m_writeIndex]` then atomically exchanges
  `m_latest` with `m_writeIndex`. The exchange returns the previous latest
  index which becomes the new `m_writeIndex` (spare).
- The consumer obtains the most recent frame by atomically exchanging
  `m_latest` with `m_consumerIndex`. The returned index is the newest frame
  to consume. If the returned value equals the current `m_consumerIndex`,
  there is no new frame.

Why triple buffer
-----------------
- With three buffers we can guarantee that the producer never writes into the
  buffer currently being read by the consumer: producer always writes into
  its private write slot. After publishing, the spare slot is swapped and
  becomes the new write slot. This rotation guarantees safety without locks
  under SPSC assumptions.

Atomicity and memory ordering
-----------------------------
- The only atomic variable is `m_latest`. Indices are swapped using
  `atomic_exchange` with `memory_order_acq_rel` to ensure producer writes are
  visible to the consumer after the exchange.

Frame dropping
--------------
- If the producer outpaces the consumer, each push will overwrite the latest
  slot and the consumer will only ever observe the most recent frame. This is
  intentional to prioritize low-latency rendering over delivering every frame.

Cache locality and false sharing
------------------------------
- Cache locality: Data that is heavily used together should be placed close
  together. In this design, each slot contains a `std::shared_ptr` to the
  frame; the producer writes only to its slot while the consumer reads only
  from its slot, improving locality of access for each thread.
- False sharing: When multiple threads write to distinct variables that
  share the same CPU cache line, cache thrashing occurs. We minimize false
  sharing by keeping `m_writeIndex` and `m_consumerIndex` as non-atomic local
  indexes (producer-only and consumer-only). The single atomic `m_latest`
  is the only shared synchronization variable.

Why avoid mutexes
------------------
- Mutexes introduce contention and potential priority inversion. For
  real-time rendering, the renderer must not block on locks held by the
  decoder. The triple-buffer algorithm provides bounded, wait-free (in the
  SPSC sense) handoff with deterministic behavior suitable for low-latency
  rendering.

Limitations
-----------
- This implementation is SPSC only. Multi-producer or multi-consumer
  scenarios require additional synchronization.
