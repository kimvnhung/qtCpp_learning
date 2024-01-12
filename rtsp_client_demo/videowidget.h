#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <qglobal.h>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QtWidgets>
#else
#include <QtGui/QWidget>
#endif


class VideoWidget : public QWidget
{
    Q_OBJECT
public:
    VideoWidget(QWidget *parent = 0);
    ~VideoWidget();

    QSize sizeHint() const;

public slots:
    void setImage(QImage* img)
    {
        mImg=img; repaint();
    }

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private:

    QImage* mImg;
};

#endif // VIDEOWIDGET_H
