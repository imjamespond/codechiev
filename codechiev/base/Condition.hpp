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
            explicit CountLatch(unsigned short );
            CountLatch();
            ~CountLatch();
            
            void reset(unsigned short);
            unsigned short latch();
            unsigned short unlatch();
            unsigned short reduce(unsigned short = 1);
            unsigned short notify(unsigned short = 0);
            unsigned short notifyall(unsigned short = 0);

          private:
            unsigned short count_;
            Condition cond_;
            Mutex mutex_;
        };
    }
}

#endif /* Condition_hpp */
