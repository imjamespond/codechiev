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

#include "base/Thread.hpp"
#include "base/Logger.hpp"

using namespace codechiev::base;

int count(0);
void print()
{

}

int main(int argc, const char * argv[]) {
    /*
    Thread* threads[8];
    
    for(int i=0;i<(sizeof(threads)/sizeof(Thread*));i++)
    {
        std::string name("thread-");
        name+=boost::lexical_cast<std::string>(i);
        auto t = new Thread(name, boost::bind(&print));
        threads[i] = t;
        t->start();
    }
    
    
    
    for(int i=0;i<(sizeof(threads)/sizeof(Thread*));i++)
    {
        threads[i]->join();
    }*/
    
    std::string str("abc");
    double db=16000000.0/9.0;
    float ft=160.0/9.0;
    LOG_INFO << str
             << ",float:" << ft
             << ", int:" << 54321
             << ", double:" << db
             << "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
    LOG_TRACE << str;
    LOG_DEBUG << str;

    return 0;
}
