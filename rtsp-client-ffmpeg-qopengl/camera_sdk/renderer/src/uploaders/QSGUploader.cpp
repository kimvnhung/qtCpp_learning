#include "QSGUploader.h"

#include <core/IFrame.h>
#include <QQuickWindow>
#include <QGuiApplication>
#include <QImage>

using namespace camera::core;

namespace camera::renderer {

QSGUploader::QSGUploader() = default;
QSGUploader::~QSGUploader() { release(); }

bool QSGUploader::uploadFrame(std::shared_ptr<IFrame> frame, void* )
{
    if (!frame) return false;

    QQuickWindow* win = QGuiApplication::allWindows().isEmpty() ? nullptr : qobject_cast<QQuickWindow*>(QGuiApplication::allWindows().first());
    if (!win) return false;

    auto fmt = frame->format();

    // Y plane
    {
        auto span = frame->planeData(0);
        if (!span.empty()) {
            int stride = frame->stride(0);
            QImage yimg(frame->width(), frame->height(), QImage::Format_Grayscale8);
            for (int y = 0; y < frame->height(); ++y) {
                const uint8_t* src = span.data() + size_t(y) * stride;
                memcpy(yimg.scanLine(y), src, static_cast<size_t>(frame->width()));
            }
            if (m_qsgTextures[0]) {
                m_qsgTextures[0]->setFiltering(QSGTexture::Linear);
                m_qsgTextures[0]->updateTexture(yimg);
            } else {
                m_qsgTextures[0] = win->createTextureFromImage(yimg);
            }
        }
    }

    // U/V upsample as in prior fallback path
    QImage uimg(frame->width(), frame->height(), QImage::Format_Grayscale8);
    QImage vimg(frame->width(), frame->height(), QImage::Format_Grayscale8);
    uimg.fill(128);
    vimg.fill(128);

    if (fmt == camera::core::PixelFormat::YUV420P) {
        auto uspan = frame->planeData(1);
        auto vspan = frame->planeData(2);
        int ustride = frame->stride(1);
        int vstride = frame->stride(2);
        int wh = (frame->width() + 1) / 2;
        int hh = (frame->height() + 1) / 2;
        for (int y = 0; y < hh; ++y) {
            const uint8_t *usrc = uspan.data() + size_t(y) * ustride;
            const uint8_t *vsrc = vspan.data() + size_t(y) * vstride;
            for (int x = 0; x < wh; ++x) {
                uint8_t U = usrc[x];
                uint8_t V = vsrc[x];
                int dstx = x * 2;
                int dsty = y * 2;
                for (int dy = 0; dy < 2; ++dy) {
                    int yy = dsty + dy;
                    if (yy >= frame->height()) continue;
                    uint8_t *uline = reinterpret_cast<uint8_t *>(uimg.scanLine(yy));
                    uint8_t *vline = reinterpret_cast<uint8_t *>(vimg.scanLine(yy));
                    if (dstx < frame->width()) uline[dstx] = U;
                    if (dstx < frame->width()) vline[dstx] = V;
                    if (dstx + 1 < frame->width()) uline[dstx + 1] = U;
                    if (dstx + 1 < frame->width()) vline[dstx + 1] = V;
                }
            }
        }
    } else if (fmt == camera::core::PixelFormat::NV12) {
        auto uvspan = frame->planeData(1);
        int uvstride = frame->stride(1);
        int wh = (frame->width() + 1) / 2;
        int hh = (frame->height() + 1) / 2;
        for (int y = 0; y < hh; ++y) {
            const uint8_t *src = uvspan.data() + size_t(y) * uvstride;
            for (int x = 0; x < wh; ++x) {
                uint8_t U = src[x * 2 + 0];
                uint8_t V = src[x * 2 + 1];
                int dstx = x * 2;
                int dsty = y * 2;
                for (int dy = 0; dy < 2; ++dy) {
                    int yy = dsty + dy;
                    if (yy >= frame->height()) continue;
                    uint8_t *uline = reinterpret_cast<uint8_t *>(uimg.scanLine(yy));
                    uint8_t *vline = reinterpret_cast<uint8_t *>(vimg.scanLine(yy));
                    if (dstx < frame->width()) uline[dstx] = U;
                    if (dstx < frame->width()) vline[dstx] = V;
                    if (dstx + 1 < frame->width()) uline[dstx + 1] = U;
                    if (dstx + 1 < frame->width()) vline[dstx + 1] = V;
                }
            }
        }
    }

    if (m_qsgTextures[1]) {
        m_qsgTextures[1]->setFiltering(QSGTexture::Linear);
        m_qsgTextures[1]->updateTexture(uimg);
    } else {
        m_qsgTextures[1] = win->createTextureFromImage(uimg);
    }

    if (m_qsgTextures[2]) {
        m_qsgTextures[2]->setFiltering(QSGTexture::Linear);
        m_qsgTextures[2]->updateTexture(vimg);
    } else {
        m_qsgTextures[2] = win->createTextureFromImage(vimg);
    }

    return true;
}

void QSGUploader::release()
{
    for (auto &t : m_qsgTextures) { if (t) { delete t; t = nullptr; } }
}

QSGTexture* QSGUploader::qsgTexture(int plane) const
{
    if (plane < 0 || plane >= 3) return nullptr;
    return m_qsgTextures[plane];
}

} // namespace camera::renderer
