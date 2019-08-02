#pragma once
#include "Noncopyable.h"

#include <pthread.h>
#include <functional>
namespace wd
{


namespace current_thread
{
    extern __thread int CacheIndex;
}

class Thread :
     Noncopyable
{
public:
    using ThreadCallBack = std::function<void()>;
    Thread(ThreadCallBack && cb, int CacheIndex = 0) : 
        m_pthid(0),
        m_CacheIndex(CacheIndex),
        m_isRunning(false),
        m_call(std::move(cb))
    {}


    void create();
    void join();
    ~Thread();

private:
    static void * threadFunc(void * arg);

private:
    pthread_t m_pthid;
    int m_CacheIndex;
    bool m_isRunning;
    ThreadCallBack m_call;
};







}//end of namespace wd

