#ifndef TEST_SERVICE_H_INCLUDED
#define TEST_SERVICE_H_INCLUDED

#include "test.pb.h"

class TestService : public com::codechiev::test::NodeService
{
public:
    void testRpc(::google::protobuf::RpcController* controller,
                       const ::com::codechiev::test::TestRequest* request,
                       ::com::codechiev::test::GenericRsp* response,
                       ::google::protobuf::Closure* done)
    {
        LOG_INFO<<"";
    }
};

#endif // TEST_SERVICE_H_INCLUDED
