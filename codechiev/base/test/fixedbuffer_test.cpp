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
    
    buffer.append("12345678");
    printf("%s\n",buffer.str());
    
    buffer.read(4);
    buffer.move();
    printf("%s\n",buffer.str());
    
    buffer.append("foo");
    printf("%s\n",buffer.str());
    
    buffer.append("bar");
    printf("%s\n",buffer.str());

    buffer.readall();
    buffer.move();
    buffer.append("helloworld");
    printf("%s\n",buffer.str());

    return 0;
}
