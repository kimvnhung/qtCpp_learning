#include "renderer/VideoMaterial.h"
#include <core/IFrame.h>
#include <QQuickWindow>
#include <QImage>
#include <QSGTextureProvider>
#include <QSGTexture>
#include <QDebug>
#include <QGuiApplication>

#include "uploaders/QSGUploader.h"
#include "../include/renderer/ITextureUploader.h"

using namespace camera::core;

namespace camera::renderer
{

    VideoMaterial::VideoMaterial()
    {
    }

    VideoMaterial::~VideoMaterial() { releaseResources(); }

    void VideoMaterial::init(void* rhiHandle)
    {
        Q_UNUSED(rhiHandle);
        // Always use QSGUploader (fallback-only build)
        m_uploader = std::make_unique<QSGUploader>();
        m_uploader->init(nullptr);
    }

    bool VideoMaterial::uploadFrame(std::shared_ptr<camera::core::IFrame> frame, void* batch)
    {
        if (!m_uploader) return false;
        return m_uploader->uploadFrame(frame, batch);
    }

    void VideoMaterial::releaseResources()
    {
        if (m_uploader) {
            m_uploader->release();
            m_uploader.reset();
        }
    }

    QSG::Texture *VideoMaterial::qsgTexture(int plane) const
    {
        // If the uploader is a QSGUploader, return its textures; otherwise null
        auto qsg = dynamic_cast<QSGUploader*>(m_uploader.get());
        if (!qsg) return nullptr;
        return qsg->qsgTexture(plane);
    }

} // namespace camera::renderer
