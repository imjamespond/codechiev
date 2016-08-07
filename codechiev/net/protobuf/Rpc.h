#ifndef RPC_H
#define RPC_H
#include <google/protobuf/service.h>
#include <google/protobuf/stubs/common.h>
#include <net/Channel.hpp>
namespace codechiev
{
    namespace net
    {
        class PbRpcChannel : public google::protobuf::RpcChannel
        {
            public:
                explicit PbRpcChannel(const channel_ptr&);
                void CallMethod(const google::protobuf::MethodDescriptor * ,
                                google::protobuf::RpcController * ,const
                                google::protobuf::Message * ,
                                google::protobuf::Message * ,
                                google::protobuf::Closure * );
            protected:
                wchannel_ptr channel_;
            private:
        };
    }
}


#endif // RPC_H
