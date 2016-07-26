//
//  Internet.h
//  codechiev
//
//  Created by metasoft on 16/7/26.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#ifndef Internet_h
#define Internet_h

#include <string>
#include <stdint.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

namespace codechiev {
    namespace net {

        
        inline void setIpAddress(const std::string& ip, uint16_t port, sockaddr_in& addr)
        {
            addr.sin_port = ::htons(port);//host to network bytes order
            ::inet_aton(ip.c_str(), &addr.sin_addr);
        }
    }
}

#endif /* Internet_h */
