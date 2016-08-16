#include "Rpc.h"
#include "test.pb.h"
using namespace codechiev::net;
using namespace google::protobuf;
using namespace com::codechiev::test;
PbRpcChannel::PbRpcChannel(const channel_ptr& channel):channel_(channel)
{
    //ctor???
}

void PbRpcChannel::CallMethod(
        const MethodDescriptor * method,
        RpcController * controller,
        const Message * request,
        Message * response,
        Closure * done)
{
        if(channel_ptr c = channel_.lock())
        {
            const GenericReq *req = static_cast<const GenericReq*>(request);
            c->write(req->SerializeAsString());
            printf("PbRpcChannel::CallMethod\n");
        }
}
