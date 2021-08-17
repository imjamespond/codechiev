#ifndef Mutex_hpp
#define Mutex_hpp

#include <pthread.h>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace learn_cpp
{
  class Mutex;
  typedef boost::shared_ptr<Mutex> MutexPtr;

  class Mutex : public boost::noncopyable
  {
  public:
    Mutex();
    Mutex(bool);
    ~Mutex();

    void Lock();
    void Unlock();

  private:
    pthread_mutex_t _mutex;
    pthread_mutexattr_t _attr;
  };


  class MutexGuard : public boost::noncopyable
  {
  public:
    explicit MutexGuard(Mutex *);
    ~MutexGuard();

  private:
    Mutex *_mutex;
  };

  class MutexLock : public boost::noncopyable
  {
  public:
    explicit MutexLock(const MutexPtr &);
    ~MutexLock();

  private:
    MutexPtr _mutex;
  };
}

#endif /* Mutex_hpp */