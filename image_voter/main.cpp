#include "mainwindow.h"
#include <QApplication>
#include <QScreen>
#include "common.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#ifdef Q_OS_ANDROID
    // Grant READ permission for Android

#endif

    // Init setting
    APP_SETTINGS();
    MainWindow w;
    w.show();
    return a.exec();
}
