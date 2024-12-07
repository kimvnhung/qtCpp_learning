#include <QtWidgets>
#include "window.hpp"
#include "common.hpp"


int main(int argc, char* argv[])
{
  QApplication app(argc, argv);
  WaterQualityWindow window;

  QSETIINGS;
  LOGD(QString("%1 = %2").arg(CSV_FILE_PATH).arg(GET_STRING(CSV_FILE_PATH)));

  // Set default theme to dark
  window.switchTheme("dark");
  window.show();

  return app.exec();
}
