
#include "../include/renderer/VideoItem.h"
#include <core/TripleBufferFrameQueue.h>
#include <core/IFrame.h>
#include <QQuickWindow>
#include <QSGRenderNode>
#include <QSGSimpleTextureNode>
#include <QLoggingCategory>

using namespace camera::core;

Q_DECLARE_LOGGING_CATEGORY(lcVideoItem)

namespace camera::renderer {

struct VideoItem::Impl {
	std::shared_ptr<TripleBufferFrameQueue> queue;
	// Render node holds the frame that will be used on render thread.
	class VideoRenderNode : public QSGRenderNode {
	public:
		VideoRenderNode() { setFlag(RequiresOwnCommandBuffer); }
		~VideoRenderNode() override = default;

		// Called on render thread
		void render(const RenderState* state) override {
			Q_UNUSED(state);
			// Acquire the frame atomically (copy shared_ptr)
			auto f = m_frame.load();
			if (!f) return;

			// Here is where QRhi texture upload and shader draw would happen.
			// For now this is a no-op placeholder to be replaced with actual
			// QRhi commands in a later step.
		}

		// Called when scene graph is cleaning up graphics resources
		void cleanupResources() override { }

		// Set a new frame for rendering (called from GUI thread during sync)
		void setFrame(std::shared_ptr<IFrame> frame) {
			m_frame.store(frame);
			markDirty(QSGNode::DirtyMaterial);
		}

		// Release GPU resources on the render thread
		void releaseResources() override {
			m_frame.store(nullptr);
		}

	private:
		std::atomic<std::shared_ptr<IFrame>> m_frame{nullptr};
	};

	Impl() : queue(std::make_shared<TripleBufferFrameQueue>()) {}
};

VideoItem::VideoItem(QQuickItem* parent)
	: QQuickItem(parent)
	, m_impl(new Impl())
{
	setFlag(ItemHasContents, true);
}

VideoItem::~VideoItem()
{
	// ensure resources released on GUI thread
	releaseResources();
	delete m_impl;
}

void VideoItem::present(std::shared_ptr<IFrame> frame) noexcept
{
	// Producer-side: push into triple buffer queue. This is lock-free.
	if (m_impl && m_impl->queue) m_impl->queue->push(std::move(frame));
}

void VideoItem::releaseResources()
{
	if (!window()) return;
	QMetaObject::invokeMethod(window(), [this]() {
		// Called on GUI thread; find node and tell it to release resources.
		auto node = static_cast<Impl::VideoRenderNode*>(updatePaintNode(nullptr, nullptr));
		if (node) node->releaseResources();
	}, Qt::QueuedConnection);
}

QSGNode* VideoItem::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*)
{
	Impl::VideoRenderNode* node = static_cast<Impl::VideoRenderNode*>(oldNode);
	if (!node) {
		node = new Impl::VideoRenderNode();
	}

	// Pull the latest frame from the triple buffer without blocking.
	if (m_impl && m_impl->queue) {
		auto latest = m_impl->queue->latestFrame();
		if (latest && *latest) {
			node->setFrame(*latest);
		}
	}

	// Request a redraw of the window so render() will be called on the render thread.
	if (window()) window()->update();

	return node;
}

void VideoItem::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
	QQuickItem::geometryChanged(newGeometry, oldGeometry);
	// Geometry changes may require resource updates; we trigger a scene graph update.
	update();
}

} // namespace camera::renderer
