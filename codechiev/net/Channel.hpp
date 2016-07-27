//
//  Channel.hpp
//  codechiev
//
//  Created by metasoft on 16/7/21.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#ifndef Channel_hpp
#define Channel_hpp

#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <sys/socket.h>
#include <boost/shared_ptr.hpp>
#include <base/FixedBuffer.h>

namespace codechiev {
    namespace net {


        static const int kBufferSize = 1024*32;
        static const int kBufferEachTimeSize = 4;
        typedef codechiev::base::FixedBuffer<kBufferSize> channel_buffer;

        class Channel
        {
        public:
            typedef std::vector<Channel*> channel_vec_t;

            Channel(int fd):fd_(fd),event_(0),connected_(false){}
            ~Channel(){close();}

            inline void setFd(int fd){fd_=fd;};
            inline void setNonBlock(){::fcntl(fd_, F_SETFL, O_NONBLOCK);}
            inline void setCloseOnExec(){::fcntl(fd_, F_SETFD, FD_CLOEXEC);}
            inline int setReuseAddr();
            inline int setKeepAlive();
            inline int getFd(){return fd_;}
            inline void setEvent(int e){event_=e;}
            inline int getEvent(){return event_;}
            inline int close(){connected_=false; return ::close(fd_);}
            inline isConnected(){return connected_;}
            inline setConnected(bool val){connected_=val;}

            inline channel_buffer* getReadBuf(){return &readbuf_;}
            inline channel_buffer* getWriteBuf(){return &writebuf_;}

            void write(const std::string&);
            void writeEvent();
        private:
            int fd_;
            int event_;
            bool connected_;

            channel_buffer readbuf_;
            channel_buffer writebuf_;
        };

        typedef Channel::channel_vec_t channel_vec;
        typedef boost::shared_ptr<Channel> channel_ptr;

        inline int
        Channel::setReuseAddr()
        {
            int on(1);
            return ::setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);
        }
        inline int
        Channel::setKeepAlive()
        {
            int on(1);
            return ::setsockopt(fd_, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof on);
        }
    }
}

#endif /* Channel_hpp */
