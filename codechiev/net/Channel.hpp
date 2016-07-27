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
            inline int getFd(){return fd_;}
            inline void setEvent(int e){event_=e;}
            inline int getEvent(){return event_;}
            inline int close(){return ::close(fd_);}

            int onRead();
            int onWrite();
        private:
            int fd_;
            int event_;
        };

        typedef Channel::chanenl_vec chanenl_vec;
        typedef boost::shared_ptr<Channel> channel_ptr;
    }
}

#endif /* Channel_hpp */
