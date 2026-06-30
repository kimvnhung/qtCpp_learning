// VideoItem.h - QQuickItem-based holder that receives VideoFrame instances
// from the decoder and presents them to the scene graph via a render node.
#pragma once

#include <QQuickItem>
#include <memory>

namespace camera::core { class IFrame; class IFrameQueue; }

namespace camera::renderer {

class VideoItem : public QQuickItem {
    Q_OBJECT
public:
    explicit VideoItem(QQuickItem* parent = nullptr);
    ~VideoItem() override;

    // Called by decoder/producer to present a frame to this item. Thread-safe
    // for the producer (it will push into an internal triple-buffer queue).
    void present(std::shared_ptr<camera::core::IFrame> frame) noexcept;

    // Release GPU resources (called from GUI/render thread)
    void releaseResources();

protected:
    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*) override;
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry);

private:
    struct Impl;
    Impl* m_impl;
};

} // namespace camera::renderer
