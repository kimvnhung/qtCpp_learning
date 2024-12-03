// COMP2811 Coursework 2: application entry point

#include <QtWidgets>
#include "window.hpp"
#include "common.hpp"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  WaterQualityWindow window;

  // Defined settings
  QSETIINGS;

  LOGD(QString("%1 = %2").arg(CSV_FILE_PATH).arg(GET_STRING(CSV_FILE_PATH)));

  window.show();

  return app.exec();
}
