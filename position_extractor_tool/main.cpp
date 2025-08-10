#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "appmodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    AppModel *appModel = new AppModel(&engine);
    qmlRegisterType<AppModel>("app",1,0,"AppModel");
    engine.rootContext()->setContextProperty("appModel", appModel);
    const QUrl url(QStringLiteral("qrc:/position_extractor_tool/Main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
