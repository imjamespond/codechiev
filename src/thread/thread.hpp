#ifndef Thread_HPP
#define Thread_HPP

#include <pthread.h>
#include <boost/function.hpp>

namespace learn_cpp
{
  class Thread
  {
  public:
    typedef void (*t_func)();
    typedef boost::function<void()> t_thread_func;
    typedef pthread_t t_pthread;

    explicit Thread(t_func); // avoid created by passing arguments?
    ~Thread();

    void start();
    void join();
    
    void runInThread();
    inline void setFunc(const t_thread_func & func) { this->thread_func_ = func; }
  private:
    t_pthread pthread_; 
    t_thread_func thread_func_;
  };
}

#endif