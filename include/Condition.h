#pragma once

#include "Mutexlock.h"
#include <error.h>
#include <pthread.h>

namespace wd
{

class Condition
{
public:
    Condition(MutexLock& mutex)
        :m_mutex(mutex)
    {
        if(pthread_cond_init(&m_cond, NULL) != 0)
        {
            perror("pthread_cond_init");
        }
    }
    ~Condition()
    {
        if(pthread_cond_destroy(&m_cond))
        {
            perror("pthread_cond_destroy");
        }
    }
    inline void wait();
    inline void notify();
    inline void notifyAll();
private:
    MutexLock& m_mutex;
    pthread_cond_t m_cond;
};

void Condition::wait()
{
    if(pthread_cond_wait(&m_cond,m_mutex.getMutexPtr()))
    {
        perror("pthread_cond_wait");
    }
}

void Condition::notify()
{
    if(pthread_cond_signal(&m_cond))
    {
        perror("pthread_cond_signal");
    }
}

void Condition::notifyAll()
{
    if(pthread_cond_broadcast(&m_cond))
    {
        perror("pthread_cond_broadcast");
    }
}


}//namespace wd

