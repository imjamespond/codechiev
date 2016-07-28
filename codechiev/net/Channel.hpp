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
#include <boost/unordered_map.hpp>
#include <base/FixedBuffer.h>

namespace codechiev {
    namespace net {

        
        static const int kBufferSize = 1024*32;
        static const int kBufferEachTimeSize = 8;
        typedef codechiev::base::FixedBuffer<kBufferSize> channel_buffer;
        
        class Channel
        {
        public:
            typedef boost::shared_ptr<Channel> channel_ptr_t;
            typedef std::vector<Channel*> channel_vec_t;
            typedef boost::unordered_map<int, channel_ptr_t> channel_map_t;
            Channel(int fd):fd_(fd){}
            
            inline void setFd(int fd){fd_=fd;};
            inline void setNonBlock(){::fcntl(fd_, F_SETFL, O_NONBLOCK);}
            inline void setCloseOnExec(){::fcntl(fd_, F_SETFD, FD_CLOEXEC);}
            inline int setReuseAddr();
            inline int setKeepAlive();
            inline int getFd(){return fd_;}
            inline void setEvent(int e){event_=e;}
            inline int getEvent(){return event_;}
            inline int close(){return ::close(fd_);}
            
            inline channel_buffer* getReadBuf(){return &readbuf_;}
            inline channel_buffer* getWriteBuf(){return &writebuf_;}
            
            void write(const std::string&);
            void writeEvent();
        private:
            int fd_;
            int event_;
            
            channel_buffer readbuf_;
            channel_buffer writebuf_;
        };
        typedef Channel::channel_map_t channel_map;
        typedef Channel::channel_vec_t channel_vec;
        typedef Channel::channel_ptr_t channel_ptr;
        
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
