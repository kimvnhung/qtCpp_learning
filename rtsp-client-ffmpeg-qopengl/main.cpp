#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFile>
#include <QImage>

#include "videoitem.h"
#include "ffmpegdecoder.h"

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<VideoItem>("Rtsp", 1, 0, "VideoItem");

    QQmlApplicationEngine engine;
    engine.loadFromModule("RtspClientFFmpegQOpenGL", "Main");

    if (engine.rootObjects().isEmpty())
    {
        return -1;
    }

    QObject *root = engine.rootObjects().first();
    QObject *videoObj = root->findChild<QObject *>("video");

    FFmpegDecoder *decoder = new FFmpegDecoder(&app);

    if (videoObj)
    {
        VideoItem *videoItem = qobject_cast<VideoItem *>(videoObj);

        if (videoItem)
        {
            QObject::connect(decoder, &FFmpegDecoder::frameReady, videoItem, &VideoItem::pushFrame, Qt::QueuedConnection);
        }
    }

    // start decoding from argv[1] or a default RTSP URL
    QString urlStr = argc > 1 ? QString::fromLocal8Bit(argv[1]) : QStringLiteral("rtsp://172.25.222.203:8554/test2");
    decoder->start(urlStr);

    return app.exec();
}
