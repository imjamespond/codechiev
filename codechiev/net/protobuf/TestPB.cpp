//
//  TestPB.cpp
//  codechie
//
//  Created by metasoft on 16/8/15.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#include "TestPB.hpp"
#include <base/Logger.hpp>

using namespace ::google::protobuf;
using namespace ::com::codechiev::test;
void
TestServiceImpl::RpcTest(RpcController *controller, const TestRequest *request, GenericRsp *response, Closure *done)
{
    LOG_INFO<<"TestServiceImpl::RpcTest";
    done->Run();
}
