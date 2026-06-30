#include "QSGUploader.h"

#include <core/IFrame.h>
#include <QQuickWindow>
#include <QGuiApplication>
#include <QSGTexture>
#include <QImage>

using namespace camera::core;

namespace camera::renderer
{

    QSGUploader::QSGUploader() = default;
    QSGUploader::~QSGUploader() { release(); }

    bool QSGUploader::uploadFrame(std::shared_ptr<IFrame> frame, void*)
    {
        if (!frame) { return false; }

        QQuickWindow* win = QGuiApplication::allWindows().isEmpty() ? nullptr : qobject_cast<QQuickWindow*>
                            (QGuiApplication::allWindows().first());

        if (!win) { return false; }

        auto fmt = frame->format();

        // Convert YUV (YUV420P or NV12) to RGB888 on CPU and upload as a single QSGTexture.
        const int w = frame->width();
        const int h = frame->height();
        QImage rgb(w, h, QImage::Format_RGB888);

        auto clamp = [](int v) {
            if (v < 0) return 0;
            if (v > 255) return 255;
            return v;
        };

        if (fmt == camera::core::PixelFormat::YUV420P) {
            auto yspan = frame->planeData(0);
            auto uspan = frame->planeData(1);
            auto vspan = frame->planeData(2);
            int ystride = frame->stride(0);
            int ustride = frame->stride(1);
            int vstride = frame->stride(2);

            for (int yy = 0; yy < h; ++yy) {
                const uint8_t* yrow = yspan.data() + size_t(yy) * ystride;
                uint8_t* prow = rgb.scanLine(yy);
                int uv_y = yy / 2;
                const uint8_t* urow = uspan.data() + size_t(uv_y) * ustride;
                const uint8_t* vrow = vspan.data() + size_t(uv_y) * vstride;

                for (int x = 0; x < w; ++x) {
                    int uv_x = x / 2;
                    int Y = int(yrow[x]);
                    int U = int(urow[uv_x]);
                    int V = int(vrow[uv_x]);
                    int C = Y - 16;
                    int D = U - 128;
                    int E = V - 128;
                    int R = clamp((298 * C + 409 * E + 128) >> 8);
                    int G = clamp((298 * C - 100 * D - 208 * E + 128) >> 8);
                    int B = clamp((298 * C + 516 * D + 128) >> 8);
                    int idx = x * 3;
                    prow[idx + 0] = static_cast<uint8_t>(R);
                    prow[idx + 1] = static_cast<uint8_t>(G);
                    prow[idx + 2] = static_cast<uint8_t>(B);
                }
            }
        } else if (fmt == camera::core::PixelFormat::NV12) {
            auto yspan = frame->planeData(0);
            auto uvspan = frame->planeData(1);
            int ystride = frame->stride(0);
            int uvstride = frame->stride(1);

            for (int yy = 0; yy < h; ++yy) {
                const uint8_t* yrow = yspan.data() + size_t(yy) * ystride;
                uint8_t* prow = rgb.scanLine(yy);
                int uv_y = yy / 2;
                const uint8_t* uvrow = uvspan.data() + size_t(uv_y) * uvstride;

                for (int x = 0; x < w; ++x) {
                    int uv_x = x / 2;
                    int Y = int(yrow[x]);
                    int U = int(uvrow[uv_x * 2 + 0]);
                    int V = int(uvrow[uv_x * 2 + 1]);
                    int C = Y - 16;
                    int D = U - 128;
                    int E = V - 128;
                    int R = clamp((298 * C + 409 * E + 128) >> 8);
                    int G = clamp((298 * C - 100 * D - 208 * E + 128) >> 8);
                    int B = clamp((298 * C + 516 * D + 128) >> 8);
                    int idx = x * 3;
                    prow[idx + 0] = static_cast<uint8_t>(R);
                    prow[idx + 1] = static_cast<uint8_t>(G);
                    prow[idx + 2] = static_cast<uint8_t>(B);
                }
            }
        } else {
            // unsupported formats: bail
            return false;
        }

        // Replace existing textures with a single RGB texture
        for (int i = 1; i < 3; ++i) { if (m_qsgTextures[i]) { delete m_qsgTextures[i]; m_qsgTextures[i] = nullptr; } }
        if (m_qsgTextures[0]) { delete m_qsgTextures[0]; m_qsgTextures[0] = nullptr; }
        m_qsgTextures[0] = win->createTextureFromImage(rgb);
        if (m_qsgTextures[0]) m_qsgTextures[0]->setFiltering(QSGTexture::Linear);

        return true;
    }

    void QSGUploader::release()
    {
        for (auto &t : m_qsgTextures) { if (t) { delete t; t = nullptr; } }
    }

    QSGTexture *QSGUploader::qsgTexture(int plane) const
    {
        if (plane < 0 || plane >= 3) { return nullptr; }

        return m_qsgTextures[plane];
    }

} // namespace camera::renderer
