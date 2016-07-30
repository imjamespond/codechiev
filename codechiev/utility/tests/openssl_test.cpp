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
#include <base/Logger.hpp>
#include <base/Time.hpp>
#include <utility/openssl/RSA.h>

using namespace codechiev::base;
using namespace codechiev::utility;

int count(0);
void print()
{

}

int main(int argc, const char * argv[]) {

    unsigned_char_vec encrytedPasswd;
    unsigned_char_vec decryptedPasswd;
    RsaUtil rsautil;

    Time now = Time::NowTm();
    for(int i(0); i<99999; i++)
    {
        int encryptLength  = \
        rsautil.publicPemEncrypt("foobar", encrytedPasswd);
        std::string base64Passwd = Base64::Base64Encode(encrytedPasswd.data(), encryptLength);
        //LOG_INFO<<"publicPemEncrypt:"<<reinterpret_cast<const char*>(encrytedPasswd.data());
        rsautil.privatePemDecrypt(reinterpret_cast<unsigned const char*>(base64Passwd.c_str()), encryptLength, decryptedPasswd);
        LOG_INFO<<"privatePemDecrypt:"<<reinterpret_cast<const char*>(decryptedPasswd.data());
        encrytedPasswd.clear();
        decryptedPasswd.clear();
    }
    LOG_INFO<<"cost millis:"<<Time::NowTm().getMillis()-now.getMillis();
    return 0;
}
