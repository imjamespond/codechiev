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
#include <exception>
#include <boost/lexical_cast.hpp>
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include "Condition.hpp"
#include "Mutex.hpp"
#include "Thread.hpp"
#include "Logger.hpp"

namespace codechiev {
    namespace base {
        
        class QueueBreak :public std::exception
        {
        public:
            explicit QueueBreak(const std::string& tx):text(tx){}
            virtual ~QueueBreak(){};
            virtual char const* what() const throw() { return text.c_str();}
            std::string text;
        };
        
        template <int ThreadNum>
        class BlockingQueue : boost::noncopyable
        {
        public:
            typedef boost::function<void()> blocking_job;
            typedef std::deque<blocking_job > blocking_queue;
            BlockingQueue():running_(false) {}
            
        public:
            typedef boost::shared_ptr<Thread> thread_ptr;
            typedef std::vector<thread_ptr> thread_vec;
            thread_vec threads_;
            Condition cond_;
            Mutex mutex_;
            blocking_queue queue_;
            bool running_;
            
        public:
            void addJob(const blocking_job &job)
            {
                MutexGuard lock(&mutex_);
                queue_.push_back(job);
                
                cond_.notify();
            }
            
            blocking_job takeJob() throw(QueueBreak)
            {
                MutexGuard lock(&mutex_);
                
                while(queue_.size()==0 && running_)
                {
                    cond_.wait(mutex_);//release mutex and block here
                }
                //printf("thread wake: %s %d, and should do some job assignment here:%d\n",
                //Thread::ThreadName().c_str(), Thread::GetTid(), count);
                if(!running_)
                {
                    throw QueueBreak("QueueBreak");
                }

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
                    try
                    {
                        blocking_job job = takeJob();
                        if(job)
                            job();//synchronize by user
                    }catch(const QueueBreak& e)
                    {
                        LOG_INFO<<e.what();
                        ::pthread_exit(NULL);
                    }catch(const std::exception &e)
                    {
                        LOG_ERROR<<e.what();
                    }
                }
            }

            void commence()
            {
                if(running_)
                {
                    return;
                }

                running_ = true;

                for(int i=0; i<ThreadNum; i++)
                {
                    std::string name="blocking-thread:";
                    name+=boost::lexical_cast<std::string>(i);
                    thread_ptr thread(new Thread(name, boost::bind(&BlockingQueue::runInThread,this)));
                    threads_.push_back(thread);

                    thread->start();
                }
            }
            
            void stop()
            {
                MutexGuard lock(&mutex_);
                if(running_)
                {
                    running_=false;
                    cond_.notifyall();
                }
            }

            ~BlockingQueue()
            {
                for(thread_vec::iterator it=threads_.begin(); it!=threads_.end(); it++)
                {
                    thread_ptr thread = *it;
                    thread->join();
                }
            }
        };


    }
}

#endif /* BlockingQueue_hpp */
