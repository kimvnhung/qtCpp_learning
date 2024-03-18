#ifndef CUSTOMWIDGET_H
#define CUSTOMWIDGET_H

#include <QMediaPlayer>
#include <QVideoWidget>
#include <QWidget>

#include <QtAVPlayer/qavvideoframe.h>


class CustomWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CustomWidget(int num_size);
    CustomWidget(const CustomWidget &other);
    CustomWidget& operator=(const CustomWidget &other);
    CustomWidget(CustomWidget &&other);
    CustomWidget& operator=(CustomWidget &&other);

    ~CustomWidget();
    void print(const char* msg);
    void play(const char* url);
public slots:
    void videoFrame(int index, const QAVVideoFrame &frame);
private:
    QList<QVideoWidget*> m_listWidget;
    QList<QMediaPlayer*> m_listPlayer;

};

#endif // CUSTOMWIDGET_H
