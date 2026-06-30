IFrameQueue
===========

Summary
-------
`IFrameQueue` is an abstract queue used to transfer frames between producer and consumer.

Design notes
------------
- The interface is intentionally minimal to allow swapping implementations: lock-free ring buffer for single-producer-single-consumer, mutex-protected multi-producer-consumer, or waitable queues.

Thread-safety
-------------
- Implementations must document their concurrency model. Consumers should tolerate approximate size reporting.
