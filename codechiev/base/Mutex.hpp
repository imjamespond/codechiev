//
//  Mutex.hpp
//  codechiev
//
//  Created by metasoft on 16/7/15.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#ifndef Mutex_hpp
#define Mutex_hpp

#include <pthread.h>
#include <boost/noncopyable.hpp>

namespace codechiev {
    namespace base {
        
        class Mutex : public boost::noncopyable
        {
        public:
            Mutex();
            ~Mutex();
            
            void lock();
            void unlock();
            
            inline pthread_mutex_t *getPthreadMutex();
            
        public:
            pthread_mutex_t mutex_;
        };
        
        class MutexGuard : public boost::noncopyable
        {
        public:
            explicit MutexGuard(Mutex*);
            ~MutexGuard();
            
        private:
            Mutex* mutex_;
        };
        
        inline pthread_mutex_t *Mutex::getPthreadMutex()
        {
            return &mutex_;
        }
    }
}

#endif /* Mutex_hpp */
