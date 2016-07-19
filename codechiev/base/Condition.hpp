//
//  Condition.hpp
//  codechiev
//
//  Created by metasoft on 16/7/18.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#ifndef Condition_hpp
#define Condition_hpp

#include <pthread.h>
#include <boost/noncopyable.hpp>
#include "Mutex.hpp"

namespace codechiev {
    namespace base {
        
        class Condition : public boost::noncopyable
        {
        public:
            Condition();
            ~Condition();
            
            void wait(Mutex&);
            void notify();
            void notifyall();
            
        private:
            pthread_cond_t cond_;
            
        };
        
    }
}

#endif /* Condition_hpp */
