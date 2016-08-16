#ifndef RPC_H
#define RPC_H
#include <google/protobuf/service.h>
#include <google/protobuf/stubs/common.h>
#include <net/Channel.hpp>
#include <boost/function.hpp>
namespace codechiev
{
    namespace net
    {
        class PbRpcChannel : public google::protobuf::RpcChannel
        {
            public:
                typedef boost::function<void(const channel_ptr&, const std::string&)> rpc_send_func;
            
                explicit PbRpcChannel(const channel_ptr&, const rpc_send_func&);
                void CallMethod(const google::protobuf::MethodDescriptor * ,
                                google::protobuf::RpcController * ,const
                                google::protobuf::Message * ,
                                google::protobuf::Message * ,
                                google::protobuf::Closure * );
                static void Callback(PbRpcChannel*, ::google::protobuf::Message *);
                inline channel_ptr getChannel(){return channel_.lock();}
            
                rpc_send_func send;
            protected:
                wchannel_ptr channel_;
            private:
        };
    }
}


#endif // RPC_H
