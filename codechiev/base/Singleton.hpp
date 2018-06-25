#ifndef Singleton_h
#define Singleton_h

#ifdef WIN32

#else
#include <pthread.h>
#include <stdlib.h>
#endif

namespace codechiev
{
namespace base
{

#ifdef WIN32
template <class T>
class Singleton
{
public:
  static T *Get()
  {
    InitOnceExecuteOnce(&once_control_, &Singleton::InitRoutine, NULL, NULL);
    return t_;
  }

  static BOOL CALLBACK InitRoutine(PINIT_ONCE InitOnce, PVOID Parameter, PVOID *lpContex)
  {
    t_ = new T;
    ::atexit(&Singleton::Destructor);
    return TRUE;
  }

  static void Destructor()
  {
    delete t_;
    t_ = NULL;
  }

protected:
private:
  static T *t_;
  static INIT_ONCE once_control_;
};

template <class T>
INIT_ONCE Singleton<T>::once_control_;
template <class T>
T *Singleton<T>::t_ = NULL;

#else
template <class T>
class Singleton
{
public:
  static T *Get()
  {
    ::pthread_once(&once_control_, &Singleton::InitRoutine);
    return t_;
  }
  static void InitRoutine()
  {
    t_ = new T;
    ::atexit(&Singleton::Destructor);
  }

  static T *GetInstance()
  {
    ::pthread_once(&once_control_, &Singleton::InitInstance);
    return t_;
  }
  static void InitInstance()
  {
    t_ = T::Instance();
    ::atexit(&Singleton::Destructor);
  }

  static void Destructor()
  {
    delete t_;
    t_ = NULL;
  }

protected:
private:
  static T *t_;
  static pthread_once_t once_control_;
};

template <class T>
pthread_once_t Singleton<T>::once_control_ = PTHREAD_ONCE_INIT;
template <class T>
T *Singleton<T>::t_ = NULL;

template <class T>
class ThreadSingleton
{
public:
  static T *Get()
  {
    ::pthread_once(&key_once_, &MakeKey);
    t_ = reinterpret_cast<T *>(::pthread_getspecific(key_));
    if (t_ == NULL)
    {
      t_ = new T;
      ::pthread_setspecific(key_, t_);
    }

    return t_;
  }

  static void MakeKey()
  {
    ::pthread_key_create(&key_, Destructor);
  }

  static void Destructor(void *)
  {
    if (t_)
      delete t_;
  }

private:
  static __thread T *t_;
  static pthread_key_t key_;
  static pthread_once_t key_once_;
};

template <class T>
__thread T *ThreadSingleton<T>::t_ = NULL;
template <class T>
pthread_key_t ThreadSingleton<T>::key_;
template <class T>
pthread_once_t ThreadSingleton<T>::key_once_;
#endif

} // namespace base
} // namespace codechiev

#endif /* Singleton_h */