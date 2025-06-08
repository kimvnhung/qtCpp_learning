#include "echo_client.h"

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);

    EchoClientApp client;

    return app.exec();
}
