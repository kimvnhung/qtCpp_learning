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

  QTimer *timer = new QTimer(&window);
  timer->setInterval(1000);
  int progressValue = -1;
  QObject::connect(timer, &QTimer::timeout, [&progressValue, &window]()
                   { progressValue = (progressValue + 1) % 101 - 1; });

  window.show();

  return app.exec();
}
