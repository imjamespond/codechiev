#include "Rpc.h"
#include "test.pb.h"
#include <net/TcpLengthCoder.h>
using namespace codechiev::net;
using namespace google::protobuf;
using namespace com::codechiev::test;
PbRpcChannel::PbRpcChannel(const channel_ptr& channel, const rpc_send_func& s):
channel_(channel), send_(s)
{}

void PbRpcChannel::CallMethod(
        const MethodDescriptor * method,
        RpcController * controller,
        const Message * request,
        Message * response,
        Closure * done)
{
        if(channel_ptr c = channel_.lock() && send_)
        {
            const GenericReq *req = static_cast<const GenericReq*>(request);
            std::string serialized = req->SerializeAsString();
            TcpLengthCoder::AppendInt32(c.get(), serialized.size());
            send_(c, serialized);
        }
}
