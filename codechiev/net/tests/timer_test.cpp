//
//  main.cpp
//  codechiev
//
//  Created by metasoft on 16/7/15.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#include <stdio.h>
#include <boost/bind.hpp>
#include <errno.h>
#include <exception>

#include <net/Timer.hpp>

using namespace codechiev;

int count(0);
void print()
{

}

int main(int argc, const char * argv[]) {

    Timer timer;
    timer.after(3000l);
    return 0;
}
