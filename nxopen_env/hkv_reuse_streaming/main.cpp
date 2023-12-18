#include <QApplication>

#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // QQmlApplicationEngine engine;
    // const QUrl url(u"qrc:/hkv_reuse_streaming/Main.qml"_qs);
    // QObject::connect(
    //     &engine,
    //     &QQmlApplicationEngine::objectCreationFailed,
    //     &app,
    //     []() { QCoreApplication::exit(-1); },
    //     Qt::QueuedConnection);
    // engine.load(url);

    MainWindow *mainWindow = new MainWindow();
    mainWindow->show();

    return app.exec();
}
