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
const int kNum=2;
int main(int argc, const char * argv[]) {

    unsigned_char_vec encrytedPasswd;
    unsigned_char_vec decryptedPasswd;
    unsigned_char_vec decryptedBase64;
    RsaUtil rsautil;

    LOG_INFO<<"test for:"<<kNum;
    Time now = Time::NowTm();
    for(int i(0); i<kNum; i++)
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
        LOG_INFO<<"privatePemDecrypt:"<<reinterpret_cast<const char*>(decryptedPasswd.data());

    }
    LOG_INFO<<"cost millis:"<<Time::NowTm().getMillis()-now.getMillis();
    return 0;
}
