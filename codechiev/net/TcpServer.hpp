//
//  TcpServer.hpp
//  codechiev
//
//  Created by metasoft on 16/7/26.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#ifndef TcpServer_hpp
#define TcpServer_hpp

#include <sys/socket.h>
#include <boost/unordered_map.hpp>
#include <boost/noncopyable.hpp>
#include "EPoll.hpp"
#include "EventLoop.h"
#include "Internet.h"

namespace codechiev {
    namespace net {
        
        class TcpServer: public boost::noncopyable
        {
        public:
            typedef boost::unordered_map<int, channel_ptr> channel_map;
            explicit TcpServer(const std::string&, uint16_t );
            
            void start();
            void stop();
            void pollEvent(const chanenl_vec&);
            
        private:
            typedef struct sockaddr sockaddr_struct;
            typedef struct sockaddr_in sockaddrin_struct;
            typedef socklen_t sock_len;
            
            Channel listench_;
            EventLoop<EPoll> loop_;
            std::string ipaddr_;
            uint16_t port_;
            sockaddrin_struct addrin_;
            sock_len addrlen_;
            channel_map channels_;
        };
    }
}


#endif /* TcpServer_hpp */
