// VideoMaterial.h
#pragma once

#include <memory>
#include <array>
#include <cstdint>
#include <QSize>

namespace QRhi {
    class QRhiTexture;
    class QRhiSampler;
    class QRhiResourceUpdateBatch;
}

class QSGTexture;

namespace camera::core { class IFrame; }

namespace camera::renderer {

// VideoMaterial: responsible for managing GPU textures for Y/U/V (or NV12 Y/UV)
// and uploading per-frame data using QRhi when available. Falls back to QSGTexture.
class VideoMaterial {
public:
    VideoMaterial();
    ~VideoMaterial();

    // Initialize with the window's QRhi (may be null if QRhi not available)
    void init(void* rhiHandle);

    // Upload frame data. Called on render thread / scene-graph thread where QRhi is valid.
    // Returns true on success.
    bool uploadFrame(std::shared_ptr<camera::core::IFrame> frame, QRhi::QRhiResourceUpdateBatch* batch);

    // Release GPU resources
    void releaseResources();

    // Access fallback QSG texture (valid after uploadFrame when using fallback)
    QSGTexture* qsgTexture(int plane) const;

private:
    // QRhi textures (Y, U, V or Y, UV)
    std::array<QRhi::QRhiTexture*, 3> m_rhiTextures{nullptr, nullptr, nullptr};
    QRhi::QRhiSampler* m_rhiSampler = nullptr;

    // Fallback QSG textures
    std::array<QSG::Texture*, 3> m_qsgTextures{nullptr, nullptr, nullptr};

    QSize m_texSize;
    bool m_usingRhi = false;
};

} // namespace camera::renderer
