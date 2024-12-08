#include <QApplication>
#include <QTranslator>

#include "generaldashboard.h"
#include "charts/chartholderbasewidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTranslator translator;
    if (translator.load("water_en_001.qm")) {
        app.installTranslator(&translator);
    }


    GeneralDashboard generalDashboard;
    generalDashboard.show();

    // ChartHolderBaseWidget chartHolderBaseWidget;
    // chartHolderBaseWidget.show();
    return app.exec();
}
