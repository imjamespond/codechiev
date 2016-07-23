//
//  Timer.hpp
//  codechiev
//
//  Created by metasoft on 16/7/21.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#ifndef Timer_hpp
#define Timer_hpp

#include "Channel.hpp"
#include <boost/noncopyable.hpp>

namespace codechiev {
    namespace net {

        class Timer : public boost::noncopyable
        {
        public:
            Timer();

            void setTime();
            void after();

        private:
            Channel channel_;
        };
    }
}

#endif /* Timer_hpp */
