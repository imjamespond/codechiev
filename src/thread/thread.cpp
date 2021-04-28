#include <stdio.h>  // perror
#include <stdlib.h> // exit

#include <thread/thread.hpp>

using namespace learn_cpp;

static void *
start_routine(void *arg)
{
  Thread *thread = static_cast<Thread *>(arg);
  thread->run();
  return NULL;
}

void Thread::start()
{
  int s;
  s = ::pthread_create(&pthread_, NULL, &start_routine, this);
  if (s != 0)
  {
    ::perror("pthread_create error");
    ::exit(EXIT_FAILURE);
  }
}

void Thread::run()
{
  if (thread_func_)
  {
    thread_func_();
  }
  ::pthread_exit(NULL);
}

void Thread::join()
{
  if (this->pthread_ == 0l)
  {
    return;
  }
  int s = ::pthread_join(this->pthread_, NULL);
  if (s != 0)
  {
    ::perror("pthread_join error");
  }
}

Thread::Thread(thread_func_t func) : pthread_(0l)
{
  this->thread_func_ = func;
}
Thread::~Thread()
{
}