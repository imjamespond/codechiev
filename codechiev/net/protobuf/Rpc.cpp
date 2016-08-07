#include "Rpc.h"
using namespace codechiev::net;
using namespace google::protobuf;
RpcChannel::RpcChannel()
{
    //ctor
}

void RpcChannel::CallMethod(
        const MethodDescriptor * method,
        RpcController * controller,
        const Message * request,
        Message * response,
        Closure * done)
{

}
