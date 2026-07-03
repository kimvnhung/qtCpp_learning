#include <QCoreApplication>
#include <QTimer>

#include "mediaplayer.h"
#include "videoconsumer.h"
#include "log.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    LOGD() << "Qt Console Application";
    MediaPlayer player;
    player.open("file_stream");
    LOGD() << "Playing video...";
    VideoConsumer consumer1;
    consumer1.setName("VC1");
    consumer1.start();
    VideoConsumer consumer2;
    consumer2.setName("VC2");
    consumer2.start();
    player.addVideoOutput(&consumer1);
    player.addVideoOutput(&consumer2);

    player.play();
    QTimer::singleShot(5000, &app, [&player]()
    {
        LOGD() << "Pausing video...";
        player.pause();
    });
    QTimer::singleShot(8000, &app, [&player]()
    {
        LOGD() << "Resuming video...";
        player.play();
    });
    QTimer::singleShot(60000, &app, [&player]()
    {
        LOGD() << "Stopping video...";
        player.stop();
    });
    return app.exec();
}
