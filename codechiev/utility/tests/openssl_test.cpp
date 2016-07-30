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

using namespace codechiev::base;

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
    typedef std::vector<unsigned char> unsignedchar_vec;

    RsaUtil rsautil;
    rsautil.publicPemEncrypt()

    return 0;
}
