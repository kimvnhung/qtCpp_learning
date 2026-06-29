#include "videoitem.h"

#include <QSGSimpleTextureNode>
#include <QQuickWindow>

VideoItem::VideoItem(QQuickItem *parent)
    : QQuickItem(parent)
{
    setFlag(ItemHasContents, true);
}

void VideoItem::pushFrame(const QImage &image)
{
    QMutexLocker locker(&m_mutex);
    m_image = image.convertToFormat(QImage::Format_RGB888);
    locker.unlock();
    update();
}

QSGNode *VideoItem::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    QSGSimpleTextureNode *node = static_cast<QSGSimpleTextureNode*>(oldNode);

    QImage img;
    {
        QMutexLocker locker(&m_mutex);
        img = m_image;
    }

    if (img.isNull())
        return oldNode;

    if (!node)
        node = new QSGSimpleTextureNode();

    // Create texture from image
    QSGTexture *tex = window()->createTextureFromImage(img);

    QSGTexture *oldTex = node->texture();
    if (oldTex && oldTex != tex) {
        delete oldTex;
    }

    node->setTexture(tex);
    node->setRect(boundingRect());

    return node;
}
