#include "mainwindow.h"
#include <QApplication>
#include "common.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // Init setting
    APP_SETTINGS();
    MainWindow w;
    w.show();
    return a.exec();
}
