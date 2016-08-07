#ifndef TEST_SERVICE_H_INCLUDED
#define TEST_SERVICE_H_INCLUDED

#include "test.pb.h"

class TestService : public com::codechiev::test::NodeService
{
public:
    void testRpc(RpcController* controller,
                       const ::com::codechiev::test::TestRequest* request,
                       ::com::codechiev::test::GenericRsp* response,
                       Closure* done)
    {
        LOG_INFO<<"";
    }
};

#endif // TEST_SERVICE_H_INCLUDED
