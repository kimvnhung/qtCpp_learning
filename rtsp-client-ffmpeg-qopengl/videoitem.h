#pragma once

#include <QQuickItem>
#include <QImage>
#include <QMutex>
#include <QQmlEngine>

class QSGSimpleTextureNode;
class QSGTexture;

class VideoItem : public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)

public:
    explicit VideoItem(QQuickItem *parent = nullptr);

    QString source() const { return m_source; }
    void setSource(const QString &s) { if (m_source == s) return; m_source = s; emit sourceChanged(); }

public slots:
    void pushFrame(const QImage &image);

signals:
    void sourceChanged();

protected:
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) override;

private:
    QImage m_image;
    QMutex m_mutex;
    QString m_source;
    QSGTexture *m_texture = nullptr;
};
