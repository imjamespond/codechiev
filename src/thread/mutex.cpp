#include <stdio.h> //NULL
#include <assert.h>
#include "mutex.hpp"

using namespace learn_cpp;

Mutex::Mutex()
{
  ::pthread_mutex_init(&_mutex, NULL);
}

Mutex::Mutex(bool recursive)
{
  if (recursive)
  {
    ::pthread_mutexattr_init(&_attr);
    ::pthread_mutexattr_settype(&_attr, PTHREAD_MUTEX_RECURSIVE);
  }

  ::pthread_mutex_init(&_mutex, &_attr);
}

Mutex::~Mutex()
{
  ::pthread_mutex_destroy(&_mutex);
}

void Mutex::Lock()
{
  ::pthread_mutex_lock(&_mutex);
}

void Mutex::Unlock()
{
  ::pthread_mutex_unlock(&_mutex);
}

MutexGuard::MutexGuard(Mutex *mutex) : _mutex(mutex)
{
  assert(_mutex);
  _mutex->Lock();
}

MutexGuard::~MutexGuard()
{
  _mutex->Unlock();
}

MutexLock::MutexLock(const MutexPtr &mutex) : _mutex(mutex)
{ 
  _mutex->Lock();
}

MutexLock::~MutexLock()
{
  _mutex->Unlock();
}