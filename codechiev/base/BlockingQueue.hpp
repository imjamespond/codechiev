//
//  BlockingQueue.hpp
//  codechiev
//
//  Created by metasoft on 16/7/18.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#ifndef BlockingQueue_hpp
#define BlockingQueue_hpp

#include <deque>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include "Condition.hpp"
#include "Mutex.hpp"
#include "Thread.hpp"

namespace codechiev {
    namespace base {
        
        template <int ThreadNum>
        class BlockingQueue : boost::noncopyable
        {
        public:
            typedef boost::function<void()> blocking_job;
            typedef std::deque<blocking_job > blocking_queue;
            BlockingQueue():runnig_(false) {}
            
        public:
            typedef boost::shared_ptr<Thread> thread_ptr;
            typedef std::vector<thread_ptr> thread_vec;
            thread_vec threads_;
            Condition cond_;
            Mutex mutex_;
            blocking_queue queue_;
            bool runnig_;
            
        public:
            void addJob(blocking_job &job)
            {
                MutexGuard lock(&mutex_);
                queue_.push_back(job);
                
                cond_.notify();
            }
            
            blocking_job takeJob()
            {
                MutexGuard lock(&mutex_);
                while(queue_.size()==0)
                {
                    cond_.wait(mutex_);//release mutex and block here
                }
                //printf("thread wake: %s %d, and should do some job assignment here:%d\n",
                //Thread::ThreadName().c_str(), Thread::GetTid(), count);
                blocking_job job = 0;
                if(queue_.size())
                {
                    job = queue_.front();
                    queue_.pop_front();
                }
                
                if(queue_.size())
                    cond_.notify();
                
                return job;
            }
            
            
            void runInThread()
            {
                while(1)
                {
                    blocking_job job = takeJob();
                    
                    if(job)
                    {
                        job();//synchronize by user
                    }
                }
            }
            
            void commence()
            {
                if(runnig_)
                {
                    return;
                }
                
                runnig_ = true;
                
                for(int i=0; i<ThreadNum; i++)
                {
                    std::string name="blocking-thread:";
                    name+=boost::lexical_cast<std::string>(i);
                    thread_ptr thread(new Thread(name, boost::bind(&BlockingQueue::runInThread,this)));
                    threads_.push_back(thread);
                    
                    thread->start();
                }
            }
            
            ~BlockingQueue()
            {
                thread_vec::iterator it;
                for(it=threads_.begin(); it!=threads_.end(); it++)
                {
                    thread_ptr thread = *it;
                    thread->join();
                }
            }
        };
        
        
    }
}

#endif /* BlockingQueue_hpp */
