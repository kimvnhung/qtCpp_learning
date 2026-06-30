// VideoMaterial.h
#pragma once

#include <memory>

namespace QSG { class Texture; }

namespace camera::core { class IFrame; }

namespace camera::renderer {

class ITextureUploader;

// VideoMaterial: thin wrapper that delegates frame uploads to an ITextureUploader
class VideoMaterial {
public:
    VideoMaterial();
    ~VideoMaterial();

    void init(void* rhiHandle);
    bool uploadFrame(std::shared_ptr<camera::core::IFrame> frame, void* batch);
    void releaseResources();

    // If using QSG fallback, expose the QSG texture for a plane
    QSG::Texture* qsgTexture(int plane) const;

private:
    std::unique_ptr<ITextureUploader> m_uploader;
};

} // namespace camera::renderer
