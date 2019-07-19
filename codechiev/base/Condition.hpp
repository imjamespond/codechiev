#ifndef Condition_hpp
#define Condition_hpp

#include <pthread.h>
#include <boost/noncopyable.hpp>
#include <base/Mutex.hpp>

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
        
        
        class CountLatch : public boost::noncopyable
        {
        public:
            explicit CountLatch(int = 1); 
            ~CountLatch();
            
            void reset(int = 1);
            int latch();
            int unlatch();
            int reduce(int = 1);
            int notify(int = 0);
            int notifyall(int = 0);

          private:
            int count_;//don't use unsigned short, or it'll cause unknown issue like the performance is poor, even block when latch
            Condition cond_;
            Mutex mutex_;
        };
    }
}

#endif /* Condition_hpp */
