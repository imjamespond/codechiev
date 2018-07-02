
#ifndef BlockedQueue_hpp
#define BlockedQueue_hpp

#include <deque>
#include <vector>
#include <exception>
#include <boost/lexical_cast.hpp>
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <base/Condition.hpp>
#include <base/Mutex.hpp>
#include <base/Thread.hpp>
#include <base/Logger.hpp>

namespace codechiev
{
namespace base
{

template <int ThreadNum>
class BlockedQueue : boost::noncopyable
{
public:
  explicit BlockedQueue(const char *name = NULL) : running_(false)
  {

    for (int i = 0; i < ThreadNum; i++)
    {
      std::string _name = "blocked-thread-";
      _name += name ? name : "";
      _name += boost::lexical_cast<std::string>(i);
      thread_ptr thread(new Thread(_name, boost::bind(&BlockedQueue::runInThread, this)));
      threads_.push_back(thread); 
    }

    start();
  }

public:
  typedef boost::shared_ptr<Thread> thread_ptr;
  typedef std::vector<thread_ptr> thread_vec;
  thread_vec threads_;

  typedef boost::function<int()> job_func;
  typedef std::deque<job_func> blocked_queue;
  blocked_queue queue_;

  Mutex mutex_;
  CountLatch latch_;
  bool running_;

public:
  void add(const job_func &job)
  {
    MutexGuard lock(&mutex_);
    queue_.push_back(job);

    latch_.unlatch();
  }

  int size( )
  {
    MutexGuard lock(&mutex_);
    return queue_.size(); 
  }

  job_func take()
  {
    MutexGuard lock(&mutex_);

    job_func job = 0;

    if (queue_.size())
    {
      job = queue_.front();
      queue_.pop_front();
    }

    return job;
  }

  void runInThread()
  {
    while (1)
    { 
      latch_.reset();
      latch_.latch(); 
      try
      {
        for (;;)
        {
          if (!running_)
          {
            return;
          }

          job_func job = take();
          if (job)
          { 
            int quit = job();
            if (quit == 1) //prevent not return value. One such mechanism, which has negligible overhead(开销), is to use a special value
            {
              return;
            }
          }
          else
          { 
            break;
          }
        }
      }
      catch (const std::exception &e)
      {
        LOG_ERROR << e.what();
        return;
      }
    } 
  }

  void start()
  {
    {
      MutexGuard lock(&mutex_);
      if (running_)
      {
        return;
      }

      running_ = true;
    }

    for (thread_vec::iterator it = threads_.begin(); it != threads_.end(); it++)
    {
      thread_ptr thread = *it;
      thread->start();
    }
  }

  void stop()
  {
    MutexGuard lock(&mutex_);
    if (running_)
    {
      running_ = false;
      latch_.unlatch();
    }
  }

  void join()
  {
    for (thread_vec::iterator it = threads_.begin(); it != threads_.end(); it++)
    {
      thread_ptr thread = *it;
      thread->join();
    }
  }

  ~BlockedQueue()
  {
    stop();
  }
};

} // namespace base
} // namespace codechiev

#endif /* BlockedQueue_hpp */