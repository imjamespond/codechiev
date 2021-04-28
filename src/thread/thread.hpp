#ifndef Thread_HPP
#define Thread_HPP

#include <pthread.h>

namespace learn_cpp
{
  class Thread
  {
  public:
    typedef void (*thread_func_t)();

    explicit Thread(thread_func_t); // avoid created by passing arguments?
    ~Thread();

    void start();
    void run();
    void join();

  private:
    pthread_t pthread_; 
    thread_func_t thread_func_;
  };
}

#endif