#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <consumer/MediaPlayer.h>
#include <consumer/VideoItem.h>
#include <consumer/AudioPlayer.h>

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<camera::consumer::VideoItem>("CameraDemo", 1, 0, "VideoItem");
    qmlRegisterType<MediaPlayer>("CameraDemo", 1, 0, "MediaPlayer");

    QQmlApplicationEngine engine;
    engine.addImportPath("qrc:/CameraDemo");

    engine.loadFromModule("CameraDemo", "Main");

    if (engine.rootObjects().isEmpty()) { return -1; }

    QObject *root = engine.rootObjects().first();

    return app.exec();
}
