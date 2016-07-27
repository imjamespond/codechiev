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

        struct InetAddress
        {
            InetAddress(const std::string& ip, uint16_t po):
            ipaddr(ip),port(po)
            {
                socklen = sizeof(struct sockaddr);
                ::memset(&sockaddrin, 0, sizeof(struct sockaddr));
                sockaddrin.sin_family = AF_INET;
                sockaddrin.sin_port = ::htons(port);//host to network bytes order
                ::inet_aton(ip.c_str(), &sockaddrin.sin_addr);
            }
            InetAddress()
            {
                socklen = 0;
                ::memset(&sockaddrin, 0, sizeof(struct sockaddr));
            }
            
            std::string ipaddr;
            uint16_t port;
            
            sockaddr_in sockaddrin;
            socklen_t socklen;
        };
        
        typedef struct InetAddress InetAddressSt;
    }
}

#endif /* Internet_h */
