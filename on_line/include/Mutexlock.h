#pragma once
#include <stdio.h>
#include <error.h>
#include <pthread.h>

namespace wd
{

class MutexLock
{
public:
    MutexLock()
        :m_nIsLocking(false)
    {
        if(pthread_mutex_init(&m_nMutex, NULL) != 0)
        {
            perror("pthread_mutex_init");
        }
    }

    ~MutexLock()
    {
        if(pthread_mutex_destroy(&m_nMutex))
        {
            perror("pthread_mutex_destroy");
        }
    }

    MutexLock(const MutexLock&) = delete;
    MutexLock& operator=(const MutexLock&) = delete;

    void lock()
    {
        if(pthread_mutex_lock(&m_nMutex))
        {
            perror("pthread_mutex_lock");
        }
        m_nIsLocking = true;
    }
    void unlock()
    {
        if(pthread_mutex_unlock(&m_nMutex))
        {
            perror("pthread_mutex_unlock");
        }
        m_nIsLocking = false;
    }
    bool isLocking() const
    {
        return m_nIsLocking;
    }
    pthread_mutex_t* getMutexPtr()
    {
        return &m_nMutex;
    }
private:
    pthread_mutex_t m_nMutex;
    bool m_nIsLocking;
};
#if 0
void MutexLock::lock()
{
    if(pthread_mutex_lock(&m_nMutex))
    {
        perror("pthread_mutex_lock");
    }
    m_nIsLocking = true;
}

void MutexLock::unlock()
{
    if(pthread_mutex_unlock(&m_nMutex))
    {
        perror("pthread_mutex_unlock");
    }
    m_nIsLocking = false;
}
#endif


class MutexLockGuard
{
public:
    MutexLockGuard(MutexLock& mutex)
        :m_nMutex(mutex)
    {
        m_nMutex.lock();
    }

    ~MutexLockGuard()
    {
        m_nMutex.unlock();
    }
private:
    MutexLock& m_nMutex;
};



}//namespace wd
