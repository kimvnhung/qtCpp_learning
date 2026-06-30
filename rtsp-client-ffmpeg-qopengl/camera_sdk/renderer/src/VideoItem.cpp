
#include "../include/renderer/VideoItem.h"
#include <core/TripleBufferFrameQueue.h>
#include <core/IFrame.h>
#include <QQuickWindow>
#include <QSGGeometryNode>
#include <QSGMaterial>
#include <QSGTexture>
#include <QOpenGLContext>
#include <QLoggingCategory>
#include "../include/renderer/VideoMaterial.h"
#if 0
#if QT_VERSION >= QT_VERSION_CHECK(6,8,0)
#include <QtGui/QRhi>
#endif
#endif

using namespace camera::core;

Q_DECLARE_LOGGING_CATEGORY(lcVideoItem)
Q_LOGGING_CATEGORY(lcVideoItem, "camera.renderer.VideoItem")

namespace camera::renderer {

// YUV to RGB shader material for QSGGeometryNode.
// This material performs YUV420P or NV12 -> RGB conversion in the fragment shader.
class YUVMaterial : public QSGMaterial {
public:
    enum TextureFormat { YUV420P, NV12 };

    YUVMaterial() : m_format(YUV420P) {}

    QSGMaterialType* type() const override {
        static QSGMaterialType t;
        return &t;
    }

    QSGMaterialShader* createShader(QSGRendererInterface::RenderMode) const override;

    // Texture pointers for each plane
    std::array<QSGTexture*, 3> textures{nullptr, nullptr, nullptr};
    TextureFormat m_format;
};

// Minimal shader implementation for YUV->RGB conversion
class YUVShader : public QSGMaterialShader {
public:
    const char* vertexShader() const override {
        return R"(
            uniform mat4 qt_Matrix;
            attribute vec4 qt_Vertex;
            attribute vec2 qt_MultiTexCoord0;
            varying vec2 texCoord;

            void main() {
                gl_Position = qt_Matrix * qt_Vertex;
                texCoord = qt_MultiTexCoord0;
            }
        )";
    }

    const char* fragmentShader() const override {
        return R"(
            varying highp vec2 texCoord;
            uniform sampler2D yPlane;
            uniform sampler2D uPlane;
            uniform sampler2D vPlane;

            void main() {
                highp float y = texture2D(yPlane, texCoord).r;
                highp float u = texture2D(uPlane, texCoord).r - 0.5;
                highp float v = texture2D(vPlane, texCoord).r - 0.5;

                highp float r = y + 1.402 * v;
                highp float g = y - 0.344136 * u - 0.714136 * v;
                highp float b = y + 1.772 * u;

                gl_FragColor = vec4(r, g, b, 1.0);
            }
        )";
    }

    void updateState(const RenderState* state, QSGMaterial* newMaterial, QSGMaterial* oldMaterial) override;
    char const* const* attributeNames() const override {
        static char const* names[] = {"qt_Vertex", "qt_MultiTexCoord0", nullptr};
        return names;
    }
};

QSGMaterialShader* YUVMaterial::createShader(QSGRendererInterface::RenderMode) const {
    return new YUVShader();
}

void YUVShader::updateState(const RenderState* state, QSGMaterial* newMaterial, QSGMaterial* oldMaterial) {
    if (state->isMatrixDirty()) {
        program()->setUniformValue("qt_Matrix", state->combinedMatrix());
    }
    auto *mat = static_cast<YUVMaterial*>(newMaterial);
    if (!oldMaterial || mat->textures != static_cast<YUVMaterial*>(oldMaterial)->textures) {
        // Bind textures (simplified; full impl would use QOpenGLShaderProgram::setUniformValue)
        for (int i = 0; i < 3; ++i) {
            if (mat->textures[i]) {
                mat->textures[i]->bind();
            }
        }
    }
}

// VideoNode: a QSGGeometryNode that renders YUV frames.
class VideoNode : public QSGGeometryNode {
public:
    VideoNode(QQuickWindow* window) : m_window(window) {
        // Initialize geometry: fullscreen quad
        QSGGeometry* g = new QSGGeometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 4);
        g->setDrawingMode(QSGGeometry::DrawTriangleStrip);
        setGeometry(g);
        setOwnsGeometry(true);

        // Initialize material
        m_material = new YUVMaterial();
        setMaterial(m_material);
        m_videoMaterial = new VideoMaterial();
        m_videoMaterial->init(window ? reinterpret_cast<void*>(window->rhi()) : nullptr);
        setFlag(OwnsGeometry, true);
        setFlag(OwnsMaterial, true);
    }

    ~VideoNode() override {
        if (m_videoMaterial) delete m_videoMaterial;
    }

    void updateGeometry(const QSizeF& size) {
        if (size.isEmpty()) return;
        auto g = geometry();
        QSGGeometry::TexturedPoint2D* v = g->vertexDataAsTexturedPoint2D();
        v[0] = {{0, 0}, {0, 0}};
        v[1] = {{float(size.width()), 0}, {1, 0}};
        v[2] = {{0, float(size.height())}, {0, 1}};
        v[3] = {{float(size.width()), float(size.height())}, {1, 1}};
        g->markVertexDataDirty();
        markDirty(DirtyGeometry);
    }

    void setFrame(std::shared_ptr<IFrame> frame) {
        m_frame = std::move(frame);
        if (!m_frame || !m_videoMaterial) {
            markDirty(DirtyMaterial);
            return;
        }

        // Perform upload on GUI thread using QSG textures
        m_videoMaterial->uploadFrame(m_frame, nullptr);
        for (int i = 0; i < 3; ++i) {
            m_material->textures[i] = reinterpret_cast<QSGTexture*>(m_videoMaterial->qsgTexture(i));
        }
        markDirty(DirtyMaterial);
    }

    std::shared_ptr<IFrame> frame() const { return m_frame; }

private:
    QQuickWindow* m_window;
    std::shared_ptr<IFrame> m_frame;
    YUVMaterial* m_material;
    VideoMaterial* m_videoMaterial = nullptr;
};

struct VideoItem::Impl {
    std::shared_ptr<TripleBufferFrameQueue> queue;
};

VideoItem::VideoItem(QQuickItem* parent)
    : QQuickItem(parent)
    , m_impl(new Impl())
{
    setFlag(ItemHasContents, true);
    m_impl->queue = std::make_shared<TripleBufferFrameQueue>();
}

VideoItem::~VideoItem()
{
    releaseResources();
    delete m_impl;
}

void VideoItem::present(std::shared_ptr<IFrame> frame) noexcept
{
    if (m_impl && m_impl->queue) {
        m_impl->queue->push(std::move(frame));
        QMetaObject::invokeMethod(this, [this]() { update(); }, Qt::QueuedConnection);
    }
}

void VideoItem::releaseResources()
{
    // Release resources is handled by scene graph cleanup
}

QSGNode* VideoItem::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*)
{
    VideoNode* node = static_cast<VideoNode*>(oldNode);
    if (!node) {
        node = new VideoNode(window());
    }

    // Update geometry
    node->updateGeometry(size());

    // Get latest frame from triple buffer (non-blocking)
    if (m_impl && m_impl->queue) {
        auto latest = m_impl->queue->latestFrame();
        if (latest) {
            node->setFrame(*latest);
        }
    }

    return node;
}

void VideoItem::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    QQuickItem::geometryChanged(newGeometry, oldGeometry);
