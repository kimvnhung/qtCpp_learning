// VideoItem.h - QQuickItem-based holder that exposes a `source` property.
#pragma once

#include <QQuickItem>

namespace camera::renderer {

class VideoItem : public QQuickItem {
    Q_OBJECT
public:
    VideoItem(QQuickItem* parent = nullptr) : QQuickItem(parent) {}
    ~VideoItem() override = default;
};

} // namespace camera::renderer
