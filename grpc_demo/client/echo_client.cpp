#include "echo_client.h"


EchoClientApp::EchoClientApp(QObject* parent)
    : QObject(parent){
    stub_ = echo::EchoService::NewStub(
        grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));

    QTimer::singleShot(0, this, &EchoClientApp::makeCall);
}

void EchoClientApp::makeCall() {
        echo::EchoRequest request;
        request.set_message("Hello from Qt console!");

        echo::EchoResponse response;
        grpc::ClientContext context;

        grpc::Status status = stub_->Echo(&context, request, &response);

        if (status.ok()) {
          qDebug("Received response: %s", response.message().c_str());
        } else {
          qDebug("RPC failed: %s", status.error_message().c_str());
        }

        QCoreApplication::quit();
}
