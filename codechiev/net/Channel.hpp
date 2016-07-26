//
//  Channel.hpp
//  codechiev
//
//  Created by metasoft on 16/7/21.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#ifndef Channel_hpp
#define Channel_hpp

#include <vector>

namespace codechiev {
    namespace net {

        class Channel
        {
        public:
            typedef std::vector<Channel*> chanenl_vec;
            
            Channel(int fd):fd_(fd){}
            inline void setFd(int fd){fd_=fd;};
            inline int getFd(){return fd_;}
            inline void setEvent(int e){event_=e;}
            inline int getEvent(){return event_;}

        private:
            int fd_;
            int event_;
        };
        
        typedef Channel::chanenl_vec chanenl_vec;
    }
}

#endif /* Channel_hpp */
