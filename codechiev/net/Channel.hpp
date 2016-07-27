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
#include <boost/shared_ptr.hpp>
#include <base/FixedBuffer.h>

namespace codechiev {
    namespace net {

        class Channel
        {
        public:
            typedef std::vector<Channel*> chanenl_vec;

            Channel(int fd):fd_(fd){}
            inline void setFd(int fd){fd_=fd;};
            inline void setNonBlock(){::fcntl(fd_, F_SETFL, O_NONBLOCK);}
            inline void setCloseOnExec(){::fcntl(fd_, F_SETFD, FD_CLOEXEC);}
            inline int setReuseAddr();
            inline void setKeepAlive(){}
            inline int getFd(){return fd_;}
            inline void setEvent(int e){event_=e;}
            inline int getEvent(){return event_;}
            inline int close(){return ::close(fd_);}

            void write(const std::string&);
            void writeEvent();
            base::FixedBuffer<kWriteBufferSize>& getWriteBuf(){return writebuf_;}
        private:
            int fd_;
            int event_;
            
            static const int kWriteBufferSize = 1024*32;
            static const int kWriteBufferEachTimeSize = 8;
            base::FixedBuffer<kWriteBufferSize> writebuf_;
        };

        typedef Channel::chanenl_vec chanenl_vec;
        typedef boost::shared_ptr<Channel> channel_ptr;
        
        inline int
        Channel::setReuseAddr()
        {
            int on(1);
            return ::setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);
        }
    }
}

#endif /* Channel_hpp */
