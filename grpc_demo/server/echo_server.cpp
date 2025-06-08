#include "echo_server.h"

Status EchoServiceImpl::Echo(ServerContext* context, const EchoRequest* request,
                             EchoResponse* reply) {
    std::cout << "Received request: " << request->message() << std::endl;
    reply->set_message("Echo: " + request->message());
    return Status::OK;
}
