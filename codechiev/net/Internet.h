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
#include <string.h>
#include <stdint.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#ifdef __linux__
#define htonll(x) ((1==htonl(1)) ? (x) : ((uint64_t)htonl((x) & 0xFFFFFFFF) << 32) | htonl((x) >> 32))
#define ntohll(x) ((1==ntohl(1)) ? (x) : ((uint64_t)ntohl((x) & 0xFFFFFFFF) << 32) | ntohl((x) >> 32))
#endif // __linux__

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

        static inline uint16_t HostToNetwork16(uint16_t val)
        {
            return ::htons(val);
        }

        static inline uint32_t HostToNetwork32(uint32_t val)
        {
            return ::htonl(val);
        }

        static inline uint64_t HostToNetwork64(uint64_t val)
        {
            return htonll(val);
        }
        
        static inline uint16_t NetworkToHost16(uint16_t val)
        {
            return ::ntohs(val);
        }
        
        static inline uint32_t NetworkToHost32(uint32_t val)
        {
            return ::ntohl(val);
        }
        
        static inline uint64_t NetworkToHost64(uint64_t val)
        {
            return ntohll(val);
        }
    }
}

#endif /* Internet_h */
