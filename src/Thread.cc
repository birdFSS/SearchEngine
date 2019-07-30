#include "../include/Thread.h"

#include <functional>
namespace wd 
{

//TLS线程局部存储
namespace current_thread
{
    __thread int CacheIndex = 0;
}

struct ThreadData
{
    using ThreadCallBack = std::function<void()>;

    ThreadData(const int & num, ThreadCallBack && cb) :
        m_CacheIndex(num),
        m_cb(cb)
    {}

    int m_CacheIndex;
    ThreadCallBack m_cb;

    void runInThread()
    {
        current_thread::CacheIndex= m_CacheIndex;
        if(m_cb)
        {
            m_cb();
        }
    }
};

void Thread::create()
{
    ThreadData* pThreadData = new ThreadData(m_CacheIndex,std::move(m_call));

    if(pthread_create(&m_pthid, nullptr, threadFunc, pThreadData) == -1)
    {
        perror("pthread_create");
    }
    m_isRunning = true;
}


void Thread::join()
{
    if(m_isRunning)
    {
        pthread_join(m_pthid, nullptr);
    }
}

Thread::~Thread()
{
    if(m_isRunning)
        pthread_detach(m_pthid);
}

void * Thread::threadFunc(void * arg)
{
    ThreadData* pThreadData= static_cast<ThreadData*>(arg);
    if(pThreadData)
        pThreadData->runInThread();

    delete pThreadData;
    return nullptr;
}

















}//wd



















