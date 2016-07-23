//
//  Channel.hpp
//  codechiev
//
//  Created by metasoft on 16/7/21.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#ifndef Channel_hpp
#define Channel_hpp

namespace codechiev {
    namespace net {

        class Channel
        {
        public:
            Channel(int fd):fd_(fd){}
            inline void setFd(int fd){fd_=fd;};
            inline int getFd(){return fd_;}

        private:
            int fd_;
        };
    }
}

#endif /* Channel_hpp */
