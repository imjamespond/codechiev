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

void handlePcreExecError(int pcreExecRet)
{
    switch(pcreExecRet) {
        case PCRE_ERROR_NOMATCH      : printf("String did not match the pattern\n");        break;
        case PCRE_ERROR_NULL         : printf("Something was null\n");                      break;
        case PCRE_ERROR_BADOPTION    : printf("A bad option was passed\n");                 break;
        case PCRE_ERROR_BADMAGIC     : printf("Magic number bad (compiled re corrupt?)\n"); break;
        case PCRE_ERROR_UNKNOWN_NODE : printf("Something kooky in the compiled re\n");      break;
        case PCRE_ERROR_NOMEMORY     : printf("Ran out of memory\n");                       break;
        default                      : printf("Unknown error\n");                           break;
    } /* end switch */
}


int main(int argc, const char * argv[]) {
    const char *kHttpRegex = "^((http[s]?|ftp):\/)?\/?([^:\/\s]+)((\/\w+)*\/)([\w\-\.]+[^#?\s]+)(.*)?(#[\w\-]+)?$";
    const char *kHttpURL = "https://www.google.com/dir/1/2/search.html?arg=0-a&arg1=1-b&arg3-c#hash";
    
    const char *pcreErrorStr;
    int pcreErrorOffset;
    pcre *pcreComipled = pcre_compile(kHttpRegex, 0, &pcreErrorStr, &pcreErrorOffset, NULL);
    if( NULL== pcre)
    {
        LOG_ERROR<<"ERROR: Could not compile "<<kHttpRegex <<" error:"<< pcreErrorStr;;
        exit(1);
    }
    
    pcre_extra *pcreExtra = pcre_study(pcreComipled, 0, &pcreErrorStr);
    if(pcreErrorStr != NULL) {
        LOG_ERROR<<"ERROR: Could not study " << kHttpRegex <<" error:"<< pcreErrorStr;
        exit(1);
    }
    
#define MAX_MATCH 30
    int subStrVec[MAX_MATCH];
    int pcreExecRet = pcre_exec(pcreCompiled,
                            pcreExtra,
                            kHttpURL,
                            strlen(kHttpURL),  // length of string
                            0,                      // Start looking at this point
                            0,                      // OPTIONS
                            subStrVec,
                            30);                    // Length of subStrVec
    
    if(pcreExecRet<0)
    {
        handlePcreExecError(pcreExecRet);
        exit(1);
    }else if(pcreExecRet==0)
    {
        LOG_WARN<<("But too many substrings were found to fit in subStrVec!\n");
        pcreExecRet = MAX_MATCH / 3;
    }
    
    const char *psubStrMatchStr;
    for(j=0; j<pcreExecRet; j++)
    {
        pcre_get_substring(kHttpURL, subStrVec, pcreExecRet, j, &(psubStrMatchStr));
        printf("Match(%2d/%2d): (%2d,%2d): '%s'\n", j, pcreExecRet-1, subStrVec[j*2], subStrVec[j*2+1], psubStrMatchStr);
    }
    // Free up the substring
    pcre_free_substring(psubStrMatchStr);
    // Free up the regular expression.
    pcre_free(pcreCompiled);
    // Free up the EXTRA PCRE value (may be NULL at this point)
    if(pcreExtra != NULL) {
#ifdef PCRE_CONFIG_JIT
        pcre_free_study(pcreExtra);
#else
        pcre_free(pcreExtra);
#endif
    
    return 0;
}
