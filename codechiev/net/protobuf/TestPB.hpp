//
//  TestPB.hpp
//  codechie
//
//  Created by metasoft on 16/8/15.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#ifndef TestPB_hpp
#define TestPB_hpp

#include "test.pb.h"

class TestServiceImpl : com::codechiev::test::TestService
{
public:
    virtual void RpcTest(::google::protobuf::RpcController* controller,
                         const ::com::codechiev::test::TestRequest* request,
                         ::com::codechiev::test::GenericRsp* response,
                         ::google::protobuf::Closure* done);
};

#endif /* TestPB_hpp */
