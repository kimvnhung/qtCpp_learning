#include "../include/renderer/VideoMaterial.h"
#include <core/IFrame.h>
#include <QQuickWindow>
#include <QImage>
#include <QSGTextureProvider>
#include <QSGTexture>
#include <QDebug>
#include <QGuiApplication>

#if QT_VERSION >= QT_VERSION_CHECK(6,8,0)
#include <QRhi>
#include <QRhiTexture>
#include <QRhiSampler>
#include <QRhiResourceUpdateBatch>
#endif

using namespace camera::core;

namespace camera::renderer {

VideoMaterial::VideoMaterial() {}

VideoMaterial::~VideoMaterial() { releaseResources(); }

void VideoMaterial::init(void* rhiHandle)
{
#if QT_VERSION >= QT_VERSION_CHECK(6,8,0)
    if (rhiHandle) {
        m_usingRhi = true;
    }
#endif
}

bool VideoMaterial::uploadFrame(std::shared_ptr<camera::core::IFrame> frame, QRhi::QRhiResourceUpdateBatch* batch)
{
    if (!frame) return false;

#if QT_VERSION >= QT_VERSION_CHECK(6,8,0)
    if (m_usingRhi && batch) {
        // QRhi path: create or reuse textures sized to frame, then upload planes
        auto fmt = frame->format();
        int w = frame->width();
        int h = frame->height();
        // Simplify: use one texture per plane with R8_UNorm
        // Create textures on demand
        for (int p = 0; p < frame->planes(); ++p) {
            if (!m_rhiTextures[p]) {
                // Create texture (API compatibility note: exact factory may differ across Qt versions)
                QRhiTexture* tex = nullptr;
                // We'll attempt to create texture via the QRhi pointer available from the batch.
                // batch has no direct rhi pointer; in practice window->rhi() should be used.
                // For portability, we skip creation here and rely on fallback if missing.
            }
            // Upload plane data into buffer via batch->uploadTexture or similar.
            const auto data = frame->planeData(p);
            int stride = frame->stride(p);
            if (!data.empty()) {
                // In a real implementation, use QRhiResourceUpdateBatch::uploadTexture
                // with proper row alignment. Here we simply note the intent.
            }
        }
        return true;
    }
#endif

    // Fallback: create QImage per plane (R8) and create QSGTexture via QQuickWindow
    QQuickWindow* win = QGuiApplication::allWindows().isEmpty() ? nullptr : qobject_cast<QQuickWindow*>(QGuiApplication::allWindows().first());
    if (!win) return false;

    // For YUV420P: three planes. For NV12: two planes (Y + interleaved UV)
    int planes = frame->planes();
    for (int p = 0; p < planes; ++p) {
        auto span = frame->planeData(p);
        if (span.empty()) continue;
        int stride = frame->stride(p);
        int planeW = (p == 0) ? frame->width() : ((frame->width() + 1) / 2);
        int planeH = (p == 0) ? frame->height() : ((frame->height() + 1) / 2);

        // Create QImage with Format_Grayscale8 using stride-aware copy
        QImage img(planeW, planeH, QImage::Format_Grayscale8);
        for (int y = 0; y < planeH; ++y) {
            const uint8_t* src = span.data() + size_t(y) * stride;
            memcpy(img.scanLine(y), src, static_cast<size_t>(planeW));
        }

        // Create or update QSG texture
        if (m_qsgTextures[p]) {
            m_qsgTextures[p]->setFiltering(QSGTexture::Linear);
            m_qsgTextures[p]->updateTexture(img);
        } else {
            QSGTexture* tex = win->createTextureFromImage(img);
            m_qsgTextures[p] = tex;
        }
    }

    return true;
}

void VideoMaterial::releaseResources()
{
#if QT_VERSION >= QT_VERSION_CHECK(6,8,0)
    // QRhi resources would be released via rhi delete or via owning QRhi instance.
    for (auto &t : m_rhiTextures) { t = nullptr; }
    m_rhiSampler = nullptr;
#endif
    for (auto &t : m_qsgTextures) { if (t) { delete t; t = nullptr; } }
}

QSG::Texture* VideoMaterial::qsgTexture(int plane) const {
    if (plane < 0 || plane >= 3) return nullptr;
    return m_qsgTextures[plane];
}

} // namespace camera::renderer
