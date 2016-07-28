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
#include <boost/noncopyable.hpp>
#include "EPoll.hpp"
#include "EventLoop.h"
#include "Internet.h"

//#undef UseEpollET

namespace codechiev {
    namespace net {

        class TcpEndpoint: public boost::noncopyable
        {
        public:

            typedef boost::function<void(Channel*)> on_connect_func;
            typedef boost::function<void(Channel*)> on_message_func;
            typedef boost::function<void(Channel*)> on_close_func;

            explicit TcpEndpoint(const std::string&, uint16_t );

            inline void setOnConnect(const on_connect_func &func){onConnect_=func;}
            inline void setOnMessage(const on_message_func &func){onMessage_=func;}
            inline void setOnClose(const on_close_func &func){onClose_=func;}

             void updateChannel(Channel *, int);
             void addChannel(Channel *, int);
             void delChannel(Channel *);

        protected:
            EventLoop<EPoll> loop_;
            InetAddressSt addr_;
            Channel channel_;

            on_connect_func onConnect_;
            on_message_func onMessage_;
            on_close_func onClose_;

        };

        class TcpServer : public TcpEndpoint
        {
        public:
            explicit TcpServer(const std::string& ip, uint16_t port);
            //as server
            void listen();
            void stop();
            void pollEvent(const channel_vec&);
            void onConnect(Channel *);
            void onClose(Channel *);
            bool onRead(Channel *);
            bool onWrite(Channel *);
            void write(Channel *, const std::string&);
            void updateChannel(Channel *, int);
        private:
            channel_map channels_;
        };

        class TcpClient : public TcpEndpoint
        {
        public:
            explicit TcpClient(const std::string& ip, uint16_t port);
            //as client
            void connect();
            void close();
            void pollEvent(const channel_vec&);
            void onConnect(Channel *);
            void onClose(Channel *);
            bool onRead(Channel *);
            bool onWrite(Channel *);
            void write( const std::string&);
            void updateChannel(Channel *, int);
        private:
        };
    }
}


#endif /* TcpServer_hpp */
