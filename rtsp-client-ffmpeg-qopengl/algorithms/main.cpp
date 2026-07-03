#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTimer>

#include "mediaplayer.h"
#include "videoconsumer.h"
#include "log.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.addImportPath("qrc:/Algorithms");

    engine.loadFromModule("Algorithms", "Main");

    if (engine.rootObjects().isEmpty()) { return -1; }

    QObject *root = engine.rootObjects().first();

    return app.exec();
}
