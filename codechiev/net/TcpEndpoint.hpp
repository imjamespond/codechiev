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
#include <base/Mutex.hpp>
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

            void setOnConnect(const on_connect_func &func){onConnect_=func;}
            void setOnData(const on_data_func &func){onData_=func;}
            void setOnClose(const on_close_func &func){onClose_=func;}

            void shut(Channel *);
            void shut(const channel_ptr&);
            void send(const channel_ptr&, const std::string&);
            void write(Channel *, const std::string&);

        protected:
            inline void updateChannel(Channel *, int);
            inline void addChannel(Channel *, int);
            inline void delChannel(Channel *);

            bool onRead(Channel *);
            bool onWrite(Channel *);
            virtual void onClose(Channel *);

            EventLoop<EPoll> loop_;
            InetAddressSt addr_;

            on_connect_func onConnect_;
            on_data_func onData_;
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

            inline int getChannelNum(){return channels_.size();}
            channel_ptr getChannel(int);
        private:
            Channel channel_;
            channel_map channels_;
            base::Mutex mutex_;
        };

        class TcpClient : public TcpEndpoint
        {
        public:
            explicit TcpClient(const std::string& ip, uint16_t port);
            //as client
            void start();
            channel_ptr connect();
            void pollEvent(const channel_vec&);
            void onConnect(Channel *);
        private:

        };

        inline void
        TcpEndpoint::updateChannel(Channel *channel, int events)
        {
            channel->setEvent(events);
            loop_.getPoll().setChannel(channel);
        }
        inline void
        TcpEndpoint::addChannel(Channel *channel, int events)
        {
            channel->setEvent(events);
            loop_.getPoll().addChannel(channel);
        }
        inline void
        TcpEndpoint::delChannel(Channel *channel)
        {
            loop_.getPoll().delChannel(channel);
        }
    }
}


#endif /* TcpServer_hpp */
