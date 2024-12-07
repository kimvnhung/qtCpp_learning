#include <QApplication>
#include <QTranslator>
#include "dashboard.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // QTranslator translator;
    // if (translator.load("water_en_001.qm")) {
    //     app.installTranslator(&translator);
    // }

    Dashboard dashboard;
    dashboard.show();

    return app.exec();
}
