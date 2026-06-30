#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFile>
#include <QImage>

#include "sdkbridge.h"
#include <camera_sdk/renderer/include/renderer/VideoItem.h>

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<camera::renderer::VideoItem>("Rtsp", 1, 0, "VideoItem");

    QQmlApplicationEngine engine;
    // Create SDK demo bridge and expose to QML
    SdkDemoBridge *bridge = new SdkDemoBridge(&app);
    engine.rootContext()->setContextProperty("sdkBridge", bridge);

    engine.loadFromModule("RtspClientFFmpegQOpenGL", "Main");

    if (engine.rootObjects().isEmpty())
    {
        return -1;
    }

    QObject *root = engine.rootObjects().first();
    // Register VideoItem instances with the bridge so frames are forwarded
    QList<camera::renderer::VideoItem*> items = root->findChildren<camera::renderer::VideoItem*>();
    for (auto* vi : items) {
        bridge->addVideoItem(vi);
    }

    // start decoding using SDK decoder with default URL
    QString urlStr = argc > 1 ? QString::fromLocal8Bit(argv[1]) : QStringLiteral("rtsp://172.25.222.203:8554/test2");
    bridge->start(urlStr);

    // Connect decoder info change to QML via context property (infoChanged signal is exposed)
    // QML can use the bound properties: decoder.fps, decoder.width, decoder.height, decoder.codec

    return app.exec();
}
