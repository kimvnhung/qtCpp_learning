#include "renderer/VideoMaterial.h"
#include <core/IFrame.h>
#include <QQuickWindow>
#include <QImage>
#include <QSGTextureProvider>
#include <QSGTexture>
#include <QDebug>
#include <QGuiApplication>

#if QT_VERSION >= QT_VERSION_CHECK(6,8,0)
    #include <QtGui/QRhi>
    #include <QtGui/QRhiTexture>
    #include <QtGui/QRhiSampler>
    #include <QtGui/QRhiResourceUpdateBatch>
#endif

using namespace camera::core;

namespace camera::renderer
{

    VideoMaterial::VideoMaterial() {}

    VideoMaterial::~VideoMaterial() { releaseResources(); }

    void VideoMaterial::init(void* rhiHandle)
    {
#if QT_VERSION >= QT_VERSION_CHECK(6,8,0)

        if (rhiHandle)
        {
            m_usingRhi = true;
            m_rhi = reinterpret_cast<QRhi *>(rhiHandle);
        }

#endif
    }

    bool VideoMaterial::uploadFrame(std::shared_ptr<camera::core::IFrame> frame, QRhiResourceUpdateBatch* batch)
    {
        if (!frame) { return false; }

        #if QT_VERSION >= QT_VERSION_CHECK(6,8,0)

        if (m_usingRhi && batch && m_rhi)
        {
            // QRhi path: create or reuse textures sized to frame, then upload planes
            auto fmt = frame->format();
            int w = frame->width();
            int h = frame->height();

            // Simplify: use one texture per plane with R8_UNorm
            // Create textures on demand
            for (int p = 0; p < frame->planes(); ++p)
            {
                int planeW = (p == 0) ? w : ((w + 1) / 2);
                int planeH = (p == 0) ? h : ((h + 1) / 2);

                // Choose texture format: R8 for single channel, RG8 for interleaved UV
                QRhiTexture::Format texFmt = QRhiTexture::R8_UNorm;

                if (fmt == camera::core::PixelFormat::NV12 && p == 1)
                {
                    texFmt = QRhiTexture::RG8_UNorm;
                }

                if (!m_rhiTextures[p])
                {
                    // Create texture via QRhi
                    m_rhiTextures[p] = m_rhi->newTexture(texFmt, QSize(planeW, planeH), 1);

                    if (m_rhiTextures[p]) { m_rhiTextures[p]->create(); }
                }
                else
                {
                    // If size changed, recreate
                    if (m_rhiTextures[p]->pixelSize() != QSize(planeW, planeH))
                    {
                        m_rhiTextures[p]->destroy();
                        delete m_rhiTextures[p];
                        m_rhiTextures[p] = m_rhi->newTexture(texFmt, QSize(planeW, planeH), 1);

                        if (m_rhiTextures[p]) { m_rhiTextures[p]->create(); }
                    }
                }

                const auto data = frame->planeData(p);
                int stride = frame->stride(p);

                if (!data.empty() && m_rhiTextures[p])
                {
                    // Upload using the provided resource update batch.
                    // Typical signature: uploadTexture(texture, dataPtr, bytesPerRow, QSize)
                    batch->uploadTexture(m_rhiTextures[p], data.data(), stride, QSize(planeW, planeH));
                }
            }

            if (!m_rhiSampler)
            {
                m_rhiSampler = m_rhi->newSampler(QRhiSampler::Nearest, QRhiSampler::ClampToEdge);

                if (m_rhiSampler) { m_rhiSampler->create(); }
            }

            return true;
        }

        #endif

        // Fallback: create QImage per plane (R8) and create QSGTexture via QQuickWindow
        QQuickWindow* win = QGuiApplication::allWindows().isEmpty() ? nullptr : qobject_cast<QQuickWindow*>
                            (QGuiApplication::allWindows().first());

        if (!win) { return false; }

        // For YUV420P: three planes. For NV12: two planes (Y + interleaved UV)
        auto fmt = frame->format();
        // We'll always produce three full-resolution QImages for Y, U, V so the shader
        // can sample using the same texture coordinates. Chromas are upsampled nearest.
        // Prepare Y image
        {
            auto span = frame->planeData(0);

            if (!span.empty())
            {
                int stride = frame->stride(0);
                QImage yimg(frame->width(), frame->height(), QImage::Format_Grayscale8);

                for (int y = 0; y < frame->height(); ++y)
                {
                    const uint8_t *src = span.data() + size_t(y) * stride;
                    memcpy(yimg.scanLine(y), src, static_cast<size_t>(frame->width()));
                }

                if (m_qsgTextures[0])
                {
                    m_qsgTextures[0]->setFiltering(QSGTexture::Linear);
                    m_qsgTextures[0]->updateTexture(yimg);
                }
                else
                {
                    m_qsgTextures[0] = win->createTextureFromImage(yimg);
                }
            }
        }

        // Prepare U and V as full-resolution images by upsampling nearest
        QImage uimg(frame->width(), frame->height(), QImage::Format_Grayscale8);
        QImage vimg(frame->width(), frame->height(), QImage::Format_Grayscale8);
        uimg.fill(128);
        vimg.fill(128);

        if (fmt == camera::core::PixelFormat::YUV420P)
        {
            // plane 1 = U (half res), plane 2 = V (half res)
            auto uspan = frame->planeData(1);
            auto vspan = frame->planeData(2);
            int ustride = frame->stride(1);
            int vstride = frame->stride(2);
            int wh = (frame->width() + 1) / 2;
            int hh = (frame->height() + 1) / 2;

            for (int y = 0; y < hh; ++y)
            {
                const uint8_t *usrc = uspan.data() + size_t(y) * ustride;
                const uint8_t *vsrc = vspan.data() + size_t(y) * vstride;

                for (int x = 0; x < wh; ++x)
                {
                    uint8_t U = usrc[x];
                    uint8_t V = vsrc[x];
                    // replicate to 2x2 block
                    int dstx = x * 2;
                    int dsty = y * 2;

                    for (int dy = 0; dy < 2; ++dy)
                    {
                        int yy = dsty + dy;

                        if (yy >= frame->height()) { continue; }

                        uint8_t *uline = reinterpret_cast<uint8_t *>(uimg.scanLine(yy));
                        uint8_t *vline = reinterpret_cast<uint8_t *>(vimg.scanLine(yy));

                        if (dstx < frame->width()) { uline[dstx] = U; }

                        if (dstx < frame->width()) { vline[dstx] = V; }

                        if (dstx + 1 < frame->width()) { uline[dstx + 1] = U; }

                        if (dstx + 1 < frame->width()) { vline[dstx + 1] = V; }
                    }
                }
            }
        }
        else if (fmt == camera::core::PixelFormat::NV12)
        {
            // plane 1 = interleaved UV (half res, RG = U,V)
            auto uvspan = frame->planeData(1);
            int uvstride = frame->stride(1);
            int wh = (frame->width() + 1) / 2;
            int hh = (frame->height() + 1) / 2;

            for (int y = 0; y < hh; ++y)
            {
                const uint8_t *src = uvspan.data() + size_t(y) * uvstride;

                for (int x = 0; x < wh; ++x)
                {
                    uint8_t U = src[x * 2 + 0];
                    uint8_t V = src[x * 2 + 1];
                    int dstx = x * 2;
                    int dsty = y * 2;

                    for (int dy = 0; dy < 2; ++dy)
                    {
                        int yy = dsty + dy;

                        if (yy >= frame->height()) { continue; }

                        uint8_t *uline = reinterpret_cast<uint8_t *>(uimg.scanLine(yy));
                        uint8_t *vline = reinterpret_cast<uint8_t *>(vimg.scanLine(yy));

                        if (dstx < frame->width()) { uline[dstx] = U; }

                        if (dstx < frame->width()) { vline[dstx] = V; }

                        if (dstx + 1 < frame->width()) { uline[dstx + 1] = U; }

                        if (dstx + 1 < frame->width()) { vline[dstx + 1] = V; }
                    }
                }
            }
        }

        // Create or update QSG textures for U and V
        if (m_qsgTextures[1])
        {
            m_qsgTextures[1]->setFiltering(QSGTexture::Linear);
            m_qsgTextures[1]->updateTexture(uimg);
        }
        else
        {
            m_qsgTextures[1] = win->createTextureFromImage(uimg);
        }

        if (m_qsgTextures[2])
        {
            m_qsgTextures[2]->setFiltering(QSGTexture::Linear);
            m_qsgTextures[2]->updateTexture(vimg);
        }
        else
        {
            m_qsgTextures[2] = win->createTextureFromImage(vimg);
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

    QSG::Texture *VideoMaterial::qsgTexture(int plane) const
    {
        if (plane < 0 || plane >= 3) { return nullptr; }

        return m_qsgTextures[plane];
    }

} // namespace camera::renderer
