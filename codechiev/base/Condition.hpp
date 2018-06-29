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
            explicit CountLatch(int );
            CountLatch();
            ~CountLatch();
            
            void reset(int);
            int latch();
            int unlatch();
            int reduce(int );
            int notify(int = 0);
            int notifyall(int = 0);

          private:
            int count_;
            Condition cond_;
            Mutex mutex_;
        };
    }
}

#endif /* Condition_hpp */
