

#include "mainwindow.h"

#include <QApplication>


extern "C" {

#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>

}

#include <libavformat/avformat.h>
#include <iostream>

#include "customModule/module_function.h"

int main(int argc, char *argv[])
{
    startRtspClient();
}
