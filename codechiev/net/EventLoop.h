//
//  EventLoop.hpp
//  codechiev
//
//  Created by metasoft on 16/7/21.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#ifndef EventLoop_hpp
#define EventLoop_hpp

#include <boost/noncopyable.hpp>
#include "../base/Thread.hpp"
#include "EPoll.hpp"

namespace codechiev {
    namespace net {
        
        template<class TPoll>
        class EventLoop : public boost::noncopyable
        {
        public:
            EventLoop()
            {
                tid_=base::Thread::Tid();
            }
            
            void loop()
            {
                assert(tid_!=base::Thread::Tid());
                
                while(1)
                {
                    
                }
            }
            
        private:
            int tid_;
            TPoll poll_;
        };
    }
}

#endif /* EventLoop_hpp */
