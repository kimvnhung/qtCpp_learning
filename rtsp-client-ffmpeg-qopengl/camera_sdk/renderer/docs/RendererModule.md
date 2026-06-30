# Renderer Module

## Overview

The renderer module integrates YUV video frames from the decoder into Qt Quick's scene graph. It provides:
- `VideoItem`: a `QQuickItem` that receives frames via `present()` and displays them using a lock-free triple buffer.
- `VideoNode`: a `QSGGeometryNode` that manages geometry, YUV→RGB shader material, and GPU texture binding.
- `YUVMaterial` and `YUVShader`: material/shader pair implementing YUV420P and NV12 to RGB conversion on GPU.

## Architecture

### Responsibilities

- **VideoItem** (GUI layer):
  - Exposes a `present(IFrame)` method callable from decoder threads (lock-free).
  - Owns a `TripleBufferFrameQueue` for decoder-to-renderer handoff.
  - Implements `QQuickItem::updatePaintNode()` to sync geometry and pull latest frames.
  - Triggers scene graph updates when new frames arrive.

- **VideoNode** (scene graph node):
  - Manages a fullscreen quad geometry in screen coordinates.
  - Holds a `YUVMaterial` that contains texture pointers and shader parameters.
  - Stores the current frame and manages GPU texture uploads (placeholder TODO).
  - Integrates with Qt Quick's rendering pipeline: scene graph batches this node with others.

- **YUVMaterial & YUVShader** (GPU rendering):
  - Material defines texture bindings and material type.
  - Shader performs YUV→RGB conversion in the fragment shader per pixel.
  - Supports BT.601 color matrix (standard for H.264/H.265).

### Threading

- **Decoder thread(s)**: Call `VideoItem::present()` (lock-free push into triple buffer). No GUI thread access.
- **GUI thread (sync phase)**: `updatePaintNode()` is called; retrieves latest frame and updates geometry/material. Synchronizes with render thread via scene graph mechanisms.
- **Render thread**: Material `updateState()` and geometry rendering. No lock acquisition. Frame and texture access happen here.

### Synchronization

**Producer (decoder) → Consumer (GUI/render):**
1. Decoder calls `present(frame)` → pushes into `TripleBufferFrameQueue` (atomic exchange).
2. GUI thread calls `updatePaintNode()` → pulls latest frame via `latestFrame()` (atomic load).
3. Frame published to `VideoNode` → stored internally.
4. Render thread draws using the frame's GPU texture bindings.

**No locks** are held during this process. The triple buffer ensures the decoder never blocks waiting for the GUI, and the GUI never blocks waiting for the render thread.

## API

### VideoItem

```cpp
class VideoItem : public QQuickItem {
    // Called by decoder threads to present a frame (lock-free).
    void present(std::shared_ptr<core::IFrame> frame) noexcept;

    // Release GPU resources (called from GUI/render cleanup).
    void releaseResources();

    // QQuickItem overrides (internal scene graph integration).
protected:
    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*) override;
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) override;
};
```

## Geometry

A fullscreen quad (4 vertices, 2 triangles) covering the VideoItem's bounding rect. Texture coordinates map (0,0) to (1,1) of the YUV plane data.

```
Screen space quad with texture coordinates:
  v0 (0,0)   uv (0,0) --------- v1 (w,0)   uv (1,0)
    |                             |
    |                             |
  v2 (0,h)   uv (0,1) --------- v3 (w,h)   uv (1,1)
```

Rendered as `GL_TRIANGLE_STRIP` in order: v0, v1, v2, v3.

## YUV→RGB Conversion

The shader samples three 2D textures (Y, U, V planes) at the current fragment's texture coordinate and applies the BT.601 color matrix:

```glsl
float y = texture2D(yPlane, texCoord).r;
float u = texture2D(uPlane, texCoord).r - 0.5;
float v = texture2D(vPlane, texCoord).r - 0.5;

float r = y + 1.402 * v;
float g = y - 0.344136 * u - 0.714136 * v;
float b = y + 1.772 * u;

gl_FragColor = vec4(r, g, b, 1.0);
```

This produces correct RGB colors for standard video content.

## Integration with Decoder

A typical usage pattern:

```cpp
// Create item
auto item = new VideoItem();

// Create decoder and frame queue
auto decoder = std::make_unique<FFmpegDecoder>();
auto frameQueue = std::make_shared<TripleBufferFrameQueue>();

// Start decoder with frame queue
DecoderConfig cfg{"rtsp://camera.local/stream"};
decoder->start(cfg, frameQueue);

// Connect decoder frames to the item
// (In practice, a manager or wrapper would do this; here showing the concept)
// When decoder produces frames, it pushes into the queue,
// and VideoItem::present() is called to consume them.
```

## Next Steps

1. **GPU texture upload**: Implement texture creation and data upload in `VideoNode::setFrame()`.
   - Create `QSGTexture` or QRhi textures for each plane.
   - Upload plane data using proper stride/pitch.
   - Handle frame format (YUV420P vs NV12) correctly.

2. **Color space variants**: Implement shader variants for BT.709, BT.2020, and limited/full range.
   - Expose `ColorSpace` and `ColorRange` metadata to shader selection.

3. **Hardware acceleration**: Add paths for GPU-backed frames (DXVA2, VDPAU, CUDA, etc.).
   - These frames may already reside on GPU; avoid unnecessary copies.

4. **QRhi backend abstraction**: Replace OpenGL-specific code with QRhi to support Vulkan, Metal, D3D.

