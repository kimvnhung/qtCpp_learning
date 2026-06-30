#pragma once

#include <memory>
#include <array>

class QSGTexture;

namespace camera::core { class IFrame; }

namespace camera::renderer {

class QSGUploader {
public:
    QSGUploader();
    ~QSGUploader();

    void init(void* rhiHandle) { Q_UNUSED(rhiHandle); }
    bool uploadFrame(std::shared_ptr<camera::core::IFrame> frame, void* batch /*unused*/);
    void release();

    QSGTexture* qsgTexture(int plane) const;

private:
    std::array<QSGTexture*,3> m_qsgTextures{nullptr, nullptr, nullptr};
};

} // namespace camera::renderer
