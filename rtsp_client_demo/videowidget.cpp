#include "videowidget.h"

#include <QtGui>


VideoWidget::VideoWidget(QWidget *parent)
    : QWidget(parent), mImg(NULL)
{
    setAutoFillBackground(false);
    setAttribute(Qt::WA_NoSystemBackground, true);

    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, Qt::black);
    setPalette(palette);

    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    setUpdatesEnabled(true);
}

VideoWidget::~VideoWidget()
{
}


QSize VideoWidget::sizeHint() const
{
    return QSize(352,288);
}


void VideoWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    if (mImg) {
        QRect videoRect = mImg->rect();
        videoRect.moveCenter(this->rect().center());

        QRect erect = event->rect();

        if (!videoRect.contains(event->rect())) {
            QRegion region = event->region();
            region = region.subtracted(videoRect);

            QBrush brush = palette().window();

            for (auto rect = region.cbegin(), end = region.cend(); rect != end; ++rect) {
                painter.fillRect(*rect, brush);
            }


        }

        painter.drawImage(videoRect, *mImg);
    } else {
        painter.fillRect(event->rect(), palette().window());
    }
}

void VideoWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
}
