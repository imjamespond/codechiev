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
#include <base/Thread.hpp>
#include <base/Logger.hpp>
#include <base/Time.hpp>
#include <utility/openssl/RSA.h>
#include <boost/bind.hpp>

using namespace codechiev::base;
using namespace codechiev::utility;


const int kNum=9999;
const int kThread=4;
void print()
{
    RsaUtil rsautil;
    int num=kNum/kThread;
    for(int i(0); i<num; i++)
    {
        unsigned_char_vec encrytedPasswd;
        unsigned_char_vec decryptedPasswd;
        unsigned_char_vec decryptedBase64;

        int encryptLength  = \
        rsautil.publicPemEncrypt("foobar", encrytedPasswd);
        std::string base64Passwd = Base64::Base64Encode(encrytedPasswd.data(), encryptLength);
        //LOG_INFO<<"publicPemEncrypt:"<<reinterpret_cast<const char*>(encrytedPasswd.data());
        Base64::Base64Decode(base64Passwd.c_str(), decryptedBase64);
        rsautil.privatePemDecrypt(decryptedBase64.data(), decryptedBase64.size(), decryptedPasswd);
        LOG_INFO<<"privatePemDecrypt:"<<reinterpret_cast<const char*>(decryptedPasswd.data())<< " size:"<< (int)decryptedPasswd.size();
        assert(decryptedPasswd.size()==7);
    }
}

int main(int argc, const char * argv[]) {

    Thread t1("t1",boost::bind(&print));
    Thread t2("t2",boost::bind(&print));
    Thread t3("t3",boost::bind(&print));
    Thread t4("t4",boost::bind(&print));

    LOG_INFO<<"test for:"<<kNum;
    Time now = Time::NowTm();
    t1.start();
    t2.start();
    t3.start();
    t4.start();
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    LOG_INFO<<"cost millis:"<<Time::NowTm().getMillis()-now.getMillis();
    return 0;
}
