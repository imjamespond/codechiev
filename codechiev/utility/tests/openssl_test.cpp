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
#include <utility/openssl/RSA.h>

using namespace codechiev::base;
using namespace codechiev::utility;

int count(0);
void print()
{

}

int main(int argc, const char * argv[]) {
    /*
    base::Base64::unsignedchar_vec encrytedPasswd;
    int encryptLength  = base::Singleton<base::RsaUtil>::get()->publicPemEncrypt(passwd, encrytedPasswd);
    std::string base64Passwd = base::Base64::Base64Encode(encrytedPasswd.data(), encryptLength);
    cmd.writer.String(base64Passwd.c_str());*/
    unsigned_char_vec encrytedPasswd;
    unsigned_char_vec decryptedPasswd;
    RsaUtil rsautil;

    int encryptLength  = \
    rsautil.publicPemEncrypt("foobar", encrytedPasswd);
    LOG_INFO<<"publicPemEncrypt:"<<reinterpret_cast<const char*>(encrytedPasswd.data());
    rsautil.privatePemDecrypt(encrytedPasswd.data(), encryptLength, decryptedPasswd);
    LOG_INFO<<"privatePemDecrypt:"<<reinterpret_cast<const char*>(decryptedPasswd.data());

    return 0;
}
