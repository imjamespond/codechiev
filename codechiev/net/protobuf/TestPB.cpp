//
//  TestPB.cpp
//  codechie
//
//  Created by metasoft on 16/8/15.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#include "TestPB.hpp"
using namespace ::google::protobuf;
using namespace ::com::codechiev::test;
void
TestServiceImpl::RpcTest(RpcController *controller, const TestRequest *request, GenericRsp *response, Closure *done)
{
    done->Run();
}
