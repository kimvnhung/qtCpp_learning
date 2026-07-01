
#include "consumer/VideoItem.h"
#include <core/TripleBufferFrameQueue.h>
#include <core/IFrame.h>
#include <QQuickWindow>
#include <QSGSimpleTextureNode>
#include <QSGTexture>
#include <QOpenGLContext>
#include <QLoggingCategory>
#include "consumer/VideoMaterial.h"

using namespace camera::core;

Q_DECLARE_LOGGING_CATEGORY(lcVideoItem)
Q_LOGGING_CATEGORY(lcVideoItem, "camera.renderer.VideoItem")

namespace camera::consumer
{

// VideoNode replacement: we use a single-texture simple node rendering an RGB texture
// produced by the uploader. This avoids relying on multi-texture shaders and
// Qt private GL APIs, keeping compatibility with Qt 6.10.
    using QSGSimpleTextureNode = ::QSGSimpleTextureNode;

    struct VideoItem::Impl
    {
        std::shared_ptr<TripleBufferFrameQueue> queue;
        VideoMaterial *videoMaterial = nullptr;
    };

    VideoItem::VideoItem(QQuickItem* parent)
        : QQuickItem(parent)
        , m_impl(new Impl())
    {
        setFlag(ItemHasContents, true);
        m_impl->queue = std::make_shared<TripleBufferFrameQueue>();
        m_impl->videoMaterial = new VideoMaterial();
        m_impl->videoMaterial->init(nullptr);
    }

    VideoItem::~VideoItem()
    {
        releaseResources();

        if (m_impl)
        {
            if (m_impl->videoMaterial) { m_impl->videoMaterial->releaseResources(); delete m_impl->videoMaterial; }

            delete m_impl;
            m_impl = nullptr;
        }
    }

    void VideoItem::present(std::shared_ptr<IFrame> frame) noexcept
    {
        if (m_impl && m_impl->queue)
        {
            m_impl->queue->push(std::move(frame));
            QMetaObject::invokeMethod(this, [this]() { update(); }, Qt::QueuedConnection);
        }
    }

    void VideoItem::releaseResources()
    {
        // Release resources is handled by scene graph cleanup
    }

    QSGNode *VideoItem::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*)
    {
        QSGSimpleTextureNode* node = static_cast<QSGSimpleTextureNode *>(oldNode);

        if (!node)
        {
            node = new QSGSimpleTextureNode();
            node->setOwnsTexture(false);
        }

        // Update rect to match item size
        node->setRect(QRectF(0, 0, size().width(), size().height()));

        // Get latest frame and upload to the material/uploader
        if (m_impl && m_impl->queue)
        {
            auto latest = m_impl->queue->latestFrame();

            if (latest && m_impl->videoMaterial)
            {
                m_impl->videoMaterial->uploadFrame(*latest, nullptr);
                QSGTexture* tex = m_impl->videoMaterial->qsgTexture(0);

                if (tex) { node->setTexture(tex); }
            }
        }

        return node;
    }

    void VideoItem::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
    {
        // QQuickItem::geometryChanged may not be accessible across Qt builds; trigger an update instead
        Q_UNUSED(newGeometry);
        Q_UNUSED(oldGeometry);
        update();
    }

} // namespace camera::renderer
