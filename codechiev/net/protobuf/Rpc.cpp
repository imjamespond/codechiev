#include "Rpc.h"
using namespace codechiev::net;
using namespace google::protobuf;
PbRpcChannel::PbRpcChannel()
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

}
