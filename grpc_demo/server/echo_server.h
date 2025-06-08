#ifndef ECHO_SERVER_H
#define ECHO_SERVER_H

#include "echo.grpc.pb.h"
#include <grpcpp/grpcpp.h>
#include <iostream>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using echo::EchoRequest;
using echo::EchoResponse;
using echo::EchoService;

class EchoServiceImpl final : public EchoService::Service {
    Status Echo(ServerContext* context, const EchoRequest* request,
                EchoResponse* reply) override;
};

#endif //ECHO_SERVER_H
