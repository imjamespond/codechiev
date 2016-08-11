//
//  main.cpp
//  codechiev
//
//  Created by metasoft on 16/7/15.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#include <stdio.h>
#include <errno.h>
#include <vector>
#include <openssl/engine.h>
#include <base/BlockingQueue.hpp>
#include <base/Condition.hpp>
#include <base/Logger.hpp>
#include <base/Time.hpp>
#include <utility/openssl/RSA.h>
#include <boost/bind.hpp>

using namespace codechiev::base;
using namespace codechiev::utility;

/*
 openssl genrsa -out private.pem 2048
 openssl rsa -in private.pem -outform PEM -pubout -out public.pem
 */

const int kNum=99;
CountLatch latch(kNum);
const int kThread=4;
typedef BlockingQueue<kThread> blocking_queue;
blocking_queue queue;

RsaUtil rsa1;

void print()
{
    unsigned_char_vec encrytedPasswd;
    unsigned_char_vec decryptedPasswd;
    unsigned_char_vec decryptedBase64;
    
    int encryptLength  = \
    rsa1.publicPemEncrypt("foobar", encrytedPasswd);
    std::string base64Passwd = Base64::Base64Encode(encrytedPasswd.data(), encryptLength);
    //LOG_INFO<<"publicPemEncrypt>>>>>>>>>>>>>>>>>>>:"<<reinterpret_cast<const char*>(encrytedPasswd.data());
    //LOG_INFO<<"Base64Encode>>>>>>>>>>>>>>>>>>>>>>>:"<<base64Passwd;
    Base64::Base64Decode(base64Passwd.c_str(), decryptedBase64);
    rsa1.privatePemDecrypt(decryptedBase64.data(), decryptedBase64.size(), decryptedPasswd);
    //LOG_INFO<<"privatePemDecrypt<<<<<<<<<<<<<<<<<<<<<:"<<reinterpret_cast<const char*>(decryptedPasswd.data())<< " size:"<< (int)decryptedPasswd.size()<<"\n\n\n\n\n\n";
    assert(decryptedPasswd.size()==7);
    
    latch.reduce(1);
}

int main(int argc, const char * argv[]) {
    LOG_INFO<<"test for:"<<kNum;
    Time began = Time::NowTm();
    
    queue.commence();
    for(int i=0; i<kNum; i++)
    {
        queue.addJob(boost::bind(&print));
    }

    latch.latch();
    Time now = Time::NowTm();
    LOG_INFO<<"cost millis:"<<now-began;
    
    queue.stop();
    //ENGINE_cleanup();
    //CONF_modules_unload();
    return 0;
}
