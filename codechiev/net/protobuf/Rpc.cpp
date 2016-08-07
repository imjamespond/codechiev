#include "Rpc.h"
using namespace codechiev::net;
using namespace google::protobuf;
PbRpcChannel::PbRpcChannel(const channel_ptr& channel):channel_(channel)
{
    //ctor
}

void PbRpcChannel::CallMethod(
        const MethodDescriptor * method,
        RpcController * controller,
        const Message * request,
        Message * response,
        Closure * done)
{
        request->set_name(msg);
        request->set_type(1212);
        request->set_id("123");
        std::string serializedTest;
        request->SerializeToString(&serializedTest);
        req.set_request(serializedTest);
        std::string serializedGeneric;
        req.SerializeToString(&serializedGeneric);

        channel_ptr c = channel_.get();
        if(c)
        {

        }
}
