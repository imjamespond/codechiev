#include "Rpc.h"
#include "test.pb.h"
#include <net/TcpLengthCoder.h>
#include <base/Logger.hpp>
using namespace codechiev::net;
using namespace google::protobuf;
using namespace com::codechiev::test;
PbRpcChannel::PbRpcChannel(const channel_ptr& channel, const rpc_send_func& s):
channel_(channel), send(s)
{}

void
PbRpcChannel::CallMethod(
        const MethodDescriptor * method,
        RpcController * controller,
        const Message * request,
        Message * response,
        Closure * done)
{
    if(channel_ptr channel = channel_.lock())
    {
        GenericReq req;
        req.set_method( method->name());
        req.set_request( request->SerializeAsString());
        
        std::string serialized = req.SerializeAsString();
        TcpLengthCoder<4>::AppendInt32(channel.get(), serialized.size());
        send(channel, serialized);
        
        LOG_DEBUG<<req.DebugString();
    }
}

void
PbRpcChannel::Callback(PbRpcChannel *pbRpc, Message *resp)
{
    if(channel_ptr channel = pbRpc->getChannel())
    {
        std::string serialized = resp->SerializeAsString();
        TcpLengthCoder<4>::AppendInt32(channel.get(), serialized.size());
        pbRpc->send(channel, resp);
    }
}
