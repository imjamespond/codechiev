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
{/*
        com::codechiev::test::GenericReq req;
        req.set_method("testRpc");
        std::string serializedTest;
        request->SerializeToString(&serializedTest);
        req.set_request(serializedTest);
        std::string serializedGeneric;
        req.SerializeToString(&serializedGeneric);
*/
        if(channel_ptr c = channel_.lock())
        {

        }
}
