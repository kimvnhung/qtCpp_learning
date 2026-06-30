#ifndef VIDEOFRAME_H
#define VIDEOFRAME_H

#include <QByteArray>


class VideoFrame
{
public:

    enum class Format
    {
        YUV420P,
        NV12
    };

    int width;

    int height;

    Format format;

    int stride[3];

    QByteArray plane[3];

    qint64 pts;
};

#endif // VIDEOFRAME_H
