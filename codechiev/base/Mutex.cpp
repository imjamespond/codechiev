//
//  Mutex.cpp
//  codechiev
//
//  Created by metasoft on 16/7/15.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#include "Mutex.hpp"
#include <assert.h>

using namespace codechiev::base;

Mutex::Mutex()
{
    ::pthread_mutex_init(&mutex_, nullptr);
}

Mutex::~Mutex()
{
    ::pthread_mutex_destroy(&mutex_);
}

void
Mutex::lock()
{
    ::pthread_mutex_lock(&mutex_);
}

void
Mutex::unlock()
{
    ::pthread_mutex_unlock(&mutex_);
}


MutexGuard::MutexGuard(Mutex* mutex):mutex_(mutex)
{
    assert(mutex_);
    mutex_->lock();
}

MutexGuard::~MutexGuard()
{
    mutex_->unlock();
}