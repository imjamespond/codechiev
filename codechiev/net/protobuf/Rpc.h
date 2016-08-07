#ifndef RPC_H
#define RPC_H
#include <google/protobuf/service.h>

namespace codechiev
{
    namespace net
    {
        class PbRpcChannel : public google::protobuf::RpcChannel
        {
            public:
                PbRpcChannel();
                void CallMethod(const google::protobuf::MethodDescriptor * ,
                                google::protobuf::RpcController * ,const
                                google::protobuf::Message * ,
                                google::protobuf::Message * ,
                                google::protobuf::Closure * );
            protected:

            private:
        };
    }
}


#endif // RPC_H
