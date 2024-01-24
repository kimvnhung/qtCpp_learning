#include <QApplication>
#include "videoplayer.h"

int main(int argc, char *argv[])
{
    const char * filename = argv[1];
    const char * format = argc > 3 ? argv[2] : "";

    QApplication a(argc, argv);
    QApplication::setQuitOnLastWindowClosed(true);

    VideoPlayer player;
    player.show();

    // filepath
    player.play(filename, format);
    // find your camera devices: ffmpeg -hide_banner -f dshow -list_devices true -i dummy
    // player.play("video=HD WebCam", format, "hflip");
    // player.play("/dev/video0", "v4l2", "hflip");

    return a.exec();
}
