//
//  main.cpp
//  codechiev
//
//  Created by metasoft on 16/7/15.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#include <stdio.h>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <errno.h>

#include "base/FixedBuffer.h"

using namespace codechiev::base;


int main(int argc, const char * argv[]) {
    
    FixedBuffer<8> buffer;
    
    printf("append:%d %s\n",buffer.append("12345678"), buffer.str());
    
    buffer.read(4);
    buffer.move();
    printf("read move %s\n", buffer.str());
    printf("append:%d %s\n",buffer.append("foo"), buffer.str());
    printf("append:%d %s\n",buffer.append("bar"), buffer.str());

    buffer.readall();
    buffer.move();
    printf("append:%d %s\n",buffer.append("helloworld"), buffer.str());

    return 0;
}
