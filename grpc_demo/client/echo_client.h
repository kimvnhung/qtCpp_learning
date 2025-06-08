#ifndef ECHO_CLIENT_H
#define ECHO_CLIENT_H

#include <QCoreApplication>
#include <QTimer>
#include <iostream>

#include <echo.grpc.pb.h>
#include <grpcpp/grpcpp.h>



using namespace std;

class EchoClientApp : public QObject {
    Q_OBJECT

public:
    EchoClientApp(QObject* parent = nullptr);
public slots:
    void makeCall();
private:
    std::unique_ptr<echo::EchoService::Stub> stub_;
};

#endif //ECHO_CLIENT_H
