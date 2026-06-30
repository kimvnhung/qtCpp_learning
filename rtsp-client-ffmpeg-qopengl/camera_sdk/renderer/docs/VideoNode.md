# VideoNode Design

## Overview

`VideoNode` is a `QSGGeometryNode`-based renderer for YUV video frames. It manages a fullscreen quad geometry, YUV→RGB shader material, and synchronization between the decoder thread and the Qt Quick scene graph.

## Architecture

### QSGGeometryNode vs QSGRenderNode

**Why QSGGeometryNode is preferred:**

1. **Integrated batching & optimization**: QSGGeometryNode participates in the scene graph's batching system. The scene graph renderer can reorder, batch, and optimize geometry nodes across the entire scene, reducing draw calls and GPU state changes. `QSGRenderNode` bypasses this and always renders in immediate mode, preventing optimization.

2. **Material consistency**: QSGGeometryNode uses `QSGMaterial` and `QSGMaterialShader` which integrate with Qt's shader caching, compilation, and state management. This ensures consistent rendering across Qt Quick and integrates well with the rendering backend (OpenGL, Vulkan, Metal, D3D via QRhi).

3. **Simpler lifecycle**: QSGGeometryNode lifecycle is well-defined and automatic: the scene graph manages creation/destruction, geometry updates, and resource cleanup. `QSGRenderNode` requires manual `render()` calls on the render thread and puts more burden on the developer.

4. **Preload/compile shaders early**: Materials are compiled during the sync phase (GUI thread), not in `render()` on the render thread. This avoids stutters from first-use shader compilation during rendering.

5. **Rendering thread pool compatibility**: Modern Qt Quick (6.x) supports render thread pools. QSGGeometryNode works across these; `QSGRenderNode` complicates multi-threaded rendering.

**When QSGRenderNode is needed**: low-level GPU control (compute shaders, direct command buffer manipulation, custom rendering primitives not representable as geometry + material).

### Node Lifecycle

1. **Creation** (`updatePaintNode` GUI thread):
   - `updatePaintNode` is called during the sync phase on the GUI/scene-graph thread.
   - If no old node exists, we create a new `VideoNode` (which internally creates a `QSGGeometry` and `YUVMaterial`).
   - Geometry and material are initialized but not yet on GPU.

2. **Geometry Update** (`updatePaintNode` GUI thread):
   - Update vertex positions to match the item's current size.
   - Mark geometry dirty with `g->markVertexDataDirty()` and `markDirty(DirtyGeometry)`.
   - The scene graph detects dirty flags and schedules GPU uploads.

3. **Frame Update** (`updatePaintNode` GUI thread):
   - Retrieve the latest frame from the triple buffer via `latestFrame()` (lock-free, non-blocking).
   - Pass it to `node->setFrame(frame)` which stores the frame and marks material dirty.

4. **Rendering** (render thread):
   - The scene graph's render thread calls `material->updateState()` to bind textures and set uniforms.
   - The geometry and material are used to issue a single draw call: textured quad with YUV→RGB shader.
   - No explicit render() override needed; QSGGeometryNode handles it via the material and geometry.

5. **Resource Cleanup** (render/GUI thread):
   - When the node is destroyed (e.g., item deleted or scene graph cleanup), QSGGeometryNode automatically cleans up geometry and material.
   - The frame `std::shared_ptr` is released, triggering buffer deref.

## Components

### YUVMaterial

A `QSGMaterial` that holds pointers to three `QSGTexture` objects (Y, U, V planes) and specifies which shader to use.

```cpp
class YUVMaterial : public QSGMaterial {
    std::array<QSGTexture*, 3> textures;
    // ...
};
```

**Key responsibilities:**
- Define the material type (for caching/reuse).
- Create an appropriate shader via `createShader()`.
- Store texture bindings and other uniforms.

### YUVShader

A `QSGMaterialShader` that implements vertex and fragment shaders for YUV→RGB conversion.

**Vertex shader:**
- Transforms quad vertices to screen space using `qt_Matrix`.
- Passes texture coordinates to the fragment shader.

**Fragment shader:**
- Samples Y, U, V planes at the current texture coordinate.
- Applies the BT.601 color space matrix to convert YUV → RGB.
- Outputs the final RGB color.

**updateState():**
- Called before rendering to update uniforms and bind textures.
- Sets the model-view-projection matrix.
- Binds plane textures to sampler units.

### VideoNode

A `QSGGeometryNode` holding the geometry (fullscreen quad), material, and current frame.

**Key methods:**
- `updateGeometry(const QSizeF& size)`: Resize the quad to match the item size.
- `setFrame(std::shared_ptr<IFrame>)`: Store a new frame for upload on the render thread.
- `frame()`: Return the current frame (used for texture upload in a future optimization step).

## Synchronization & Threading

### Thread model

- **GUI thread**: Owns the scene graph sync phase. `updatePaintNode()` is called here to create/update nodes, update geometry, and publish frames to the material.
- **Render thread**: Owned by Qt Quick's renderer. Executes `material->updateState()` and issues GPU draw calls. Must not access GUI-only data.
- **Decoder thread(s)**: Produce frames and call `VideoItem::present()` (lock-free push into triple buffer).

### Synchronization guarantee

1. Decoder pushes frame into `TripleBufferFrameQueue` (lock-free, atomic exchange on index).
2. GUI thread calls `updatePaintNode()` and retrieves latest frame via `latestFrame()` (atomic read, lock-free).
3. Frame is passed to `VideoNode::setFrame()` which stores it internally.
4. Render thread accesses the frame when calling material's `updateState()` and issues draw.
5. Frame reference is held by the node until a new frame is set or the node is destroyed.

**No locks** are acquired in this path, enabling low-latency presentation without GUI/render thread contention.

## GPU Resource Management

### Texture allocation & upload

Currently a TODO placeholder in `VideoNode::setFrame()`. The complete implementation will:
- Check if a GPU texture already exists for the current frame or if we need a new one.
- If needed, create `QSGTexture` (or `QOpenGLTexture` if using OpenGL directly) or use QRhi for abstraction.
- Upload plane data using `QSGTexture::setData()` or a `QRhiResourceUpdateBatch`.
- Store texture pointers in the material so the shader can bind them.

### Cleanup

When a node is destroyed (due to scene graph cleanup or explicit deletion), the `QSGGeometryNode` destructor releases the geometry and material. The material destructor releases textures. The frame's `std::shared_ptr` is released, decrementing refcounts and potentially freeing the `VideoBuffer`.

## Geometry Definition

A fullscreen quad with texture coordinates:

```
Vertices (screen space):
  (0, 0) --------- (w, 0)
    |               |
    |               |
  (0, h) --------- (w, h)

Texture coordinates:
  (0, 0) --------- (1, 0)
    |               |
    |               |
  (0, 1) --------- (1, 1)
```

Drawn as `DrawTriangleStrip` (two triangles = 4 vertices).

## Color Space & YUV Range

The shader implements BT.601 YUV→RGB, the standard for SDTV and most H.264/H.265 streams:

```
R = Y + 1.402 * (V - 0.5)
G = Y - 0.344136 * (U - 0.5) - 0.714136 * (V - 0.5)
B = Y + 1.772 * (U - 0.5)
```

This assumes full-range YUV (0-255). For limited-range (16-235), preprocessing in the fragment shader or in the upload step would be needed. The `ColorRange` metadata in `FrameMetadata` can guide this in a future optimization.

## Next Steps

1. **Texture upload implementation**: Replace the `setFrame()` TODO with actual GPU texture creation and data upload using QRhi or OpenGL.
2. **Plane layout handling**: Properly read strides and plane offsets from the `VideoFrame` and configure GPU textures correctly.
3. **Color space matrix selection**: Use `ColorSpace` and `ColorRange` from the frame metadata to select the correct shader variant (BT.601, BT.709, BT.2020, limited/full range).
4. **Hardware acceleration**: Add code paths for GPU-backed frames (DXVA2, VDPAU, CUDA) where available.

