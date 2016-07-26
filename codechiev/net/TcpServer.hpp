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
#include <boost/function.hpp>
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
            typedef boost::function<void> on_connect_func;
            typedef boost::function<void> on_message_func;
            typedef boost::function<void> on_close_func;
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

            on_connect_func onConnect_;
            on_message_func onMessage_;
            on_close_func onClose_;
        };
    }
}


#endif /* TcpServer_hpp */
