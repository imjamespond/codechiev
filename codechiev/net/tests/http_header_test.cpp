//
//  main.cpp
//  codechiev
//
//  Created by metasoft on 16/7/15.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#include <pcre.h>
#include <boost/bind.hpp>
#include <errno.h>
#include <exception>
#include <net/EventLoop.h>
#include <net/Timer.hpp>
#include <base/Thread.hpp>
#include <base/Time.hpp>
#include <base/Logger.hpp>

using namespace codechiev::base;


int main(int argc, const char * argv[]) {
    const char *kHttpRegex = "^((http[s]?|ftp):\/)?\/?([^:\/\s]+)((\/\w+)*\/)([\w\-\.]+[^#?\s]+)(.*)?(#[\w\-]+)?$";
    const char *kHttpURL = "https://www.google.com/dir/1/2/search.html?arg=0-a&arg1=1-b&arg3-c#hash";
    
    const char *pcreErrorStr;
    int pcreErrorOffset;
    pcre *pcre = pcre_compile(kHttpRegex, 0, &pcreErrorStr, &pcreErrorOffset, NULL);
    if( NULL== pcre)
    {
        LOG_ERROR<<"ERROR: Could not compile "<<kHttpRegex <<" error:"<< pcreErrorStr;;
        exit(1);
    }
    
    pcre_extra *pcreExtra =   pcreExtra = pcre_study(pcre, 0, &pcreErrorStr);
    if(pcreErrorStr != NULL) {
        LOG_ERROR<<"ERROR: Could not study " << kHttpRegex <<" error:"<< pcreErrorStr;
        exit(1);
    }
    return 0;
}
