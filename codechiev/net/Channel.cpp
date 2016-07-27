//
//  Channel.cpp
//  codechiev
//
//  Created by metasoft on 16/7/21.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#include "Channel.hpp"
#include <base/Logger.hpp>

using namespace codechiev::base;
using namespace codechiev::net;

void
Channel::write(const std::string& msg)
{
    writebuf_.append(msg.c_str());
}

void
Channel::writeEvent()
{
    LOG_DEBUG<<"writeEvent fd:"<< fd_ <<" readable:"<<writebuf_.readable();
}