#include "mainwindow.h"

#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow mainwindow;
    mainwindow.show();

    return app.exec();
}
