#pragma once

#include <memory>
#include <array>

#include <renderer/ITextureUploader.h>

class QSGTexture;

namespace camera::core { class IFrame; }

namespace camera::renderer {

class QSGUploader : public ITextureUploader {
public:
    QSGUploader();
    ~QSGUploader() override;

    void init(void* rhiHandle) override { (void)rhiHandle; }
    bool uploadFrame(std::shared_ptr<camera::core::IFrame> frame, void* batch /*unused*/) override;
    void release() override;

    QSGTexture* qsgTexture(int plane) const;

private:
    std::array<QSGTexture*,3> m_qsgTextures{nullptr, nullptr, nullptr};
};

} // namespace camera::renderer
