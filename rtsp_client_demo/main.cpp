
#include "module_function.h"

#include <QApplication>


extern "C" {

#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>

}

#include <libavformat/avformat.h>



int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    startRtspClient();

    return a.exec();
}
