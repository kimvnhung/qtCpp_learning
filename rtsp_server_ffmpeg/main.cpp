#include <QCoreApplication>
#include "rtspserver.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    RTSPServer server;
    if (!server.start("0.0.0.0", 8554, "D:/Codes/NGS_documents/videoplayback.mp4")) {
        return -1;
    }

    return a.exec();
}
