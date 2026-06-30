Camera SDK
==========

This folder contains a modular Camera SDK implemented in modern C++20, designed
to integrate Qt6 and FFmpeg for high-performance YUV rendering using QRhi.

Structure
---------
- `core/` — core interfaces and data types (IFrame, VideoFrame, IFrameQueue, IDecoder).
- `ffmpeg/` — FFmpeg-based decoder implementation and packet recording.
- `renderer/` — QRhi-based renderer, `VideoItem`/`VideoNode` and shaders.
- `demo/` — minimal Qt Quick demo application.

Build
-----
Use Conan to install dependencies and CMake to configure & build.

Example:

```sh
conan install . --output-folder=build --build=missing
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
cmake --install build --prefix /usr/local
```
