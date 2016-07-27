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
            typedef boost::function<void(Channel*)> on_connect_func;
            typedef boost::function<void(const char*)> on_message_func;
            typedef boost::function<void(Channel*)> on_close_func;

            explicit TcpServer(const std::string&, uint16_t );

            void start();
            void stop();
            void pollEvent(const chanenl_vec&);

            inline void setOnConnect(const on_connect_func &func){onConnect_=func;}
            inline void setOnMessage(const on_message_func &func){onMessage_=func;}
            inline void setOnClose(const on_close_func &func){onClose_=func;}

        private:
            Channel listench_;
            EventLoop<EPoll> loop_;

            InetAddressSt addr_;
            channel_map channels_;

            on_connect_func onConnect_;
            on_message_func onMessage_;
            on_close_func onClose_;

            void onConnect(Channel *);
            void onClose(Channel *);
            void onRead(Channel *);
            void onWrite(Channel *);
            void write(Channel *, const std::string&);
        };
    }
}


#endif /* TcpServer_hpp */
