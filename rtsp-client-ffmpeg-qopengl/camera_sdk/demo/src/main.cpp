#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "DemoBridge.h"
#include <camera_sdk/renderer/include/renderer/VideoItem.h>

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<camera::renderer::VideoItem>("Rtsp", 1, 0, "VideoItem");

    QQmlApplicationEngine engine;
    DemoBridge *bridge = new DemoBridge(&app);
    engine.rootContext()->setContextProperty("sdkBridge", bridge);

    engine.loadFromModule("CameraSdkDemo", "Main");

    if (engine.rootObjects().isEmpty()) return -1;

    QObject *root = engine.rootObjects().first();
    QList<camera::renderer::VideoItem*> items = root->findChildren<camera::renderer::VideoItem*>();
    for (auto* vi : items) bridge->addVideoItem(vi);

    QString urlStr = argc > 1 ? QString::fromLocal8Bit(argv[1]) : QStringLiteral("rtsp://172.25.222.203:8554/test2");
    bridge->start(urlStr);

    return app.exec();
}
