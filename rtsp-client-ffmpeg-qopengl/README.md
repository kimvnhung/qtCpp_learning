# rtsp-client-ffmpeg-qopengl

Simple example showing RTSP decode with FFmpeg and rendering to Qt Quick (`QQuickItem` → `QSGTexture`).

Prerequisites
- Qt 6 (with Qt Quick/QML)
- FFmpeg dev libs available via pkg-config (libavformat, libavcodec, libswscale, libavutil)
- CMake >= 3.14

Build

```bash
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build .
```

Run

```bash
# default URL is embedded in the binary; override with an RTSP URL
./rtsp-client-ffmpeg-qopengl rtsp://<your_rtsp_url>
```

Notes
- This example is intentionally minimal. It decodes frames in a background thread and passes `QImage` frames to a `VideoItem` which uploads them to the scene-graph as `QSGTexture` objects.
- For production use consider using a GL texture upload path (avoids CPU copies), robust reconnection logic, FPS throttling, and proper resource lifetime management.
