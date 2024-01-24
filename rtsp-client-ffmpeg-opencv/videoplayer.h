#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QWidget>
#include <QPainter>
#include <QImage>
#include "mediadecoder.h"
#include "fmt/format.h"

class VideoPlayer : public QWidget {
    Q_OBJECT

public:
    explicit VideoPlayer(QWidget* parent = nullptr);
    ~VideoPlayer() override { av_frame_free(&frame_); };

    bool play(const std::string& name, const std::string& fmt = "", const std::string& filter_descr = "");

protected:
    void paintEvent(QPaintEvent*) override
    {
        std::lock_guard lock(mtx_);

        if (frame_ && (frame_->format == AV_PIX_FMT_RGB24)){
            QPainter painter(this);
            painter.drawImage(rect(), QImage(static_cast<const uchar*>(frame_->data[0]), frame_->width, frame_->height, QImage::Format_RGB888));
        }
    }

    std::unique_ptr<MediaDecoder> decoder_{ nullptr };

    AVFrame *frame_{ nullptr };
    std::mutex mtx_;
};


#endif // VIDEOPLAYER_H
