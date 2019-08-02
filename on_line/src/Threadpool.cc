#include "../include/Threadpool.h"
#include "../include/Thread.h"
#include "../include/CacheManager.h"
#include <unistd.h>

#include <memory>

using std::unique_ptr;


namespace wd
{

Threadpool::Threadpool(size_t threadNum, size_t queSize) :
    m_queSize(queSize),
    m_threadNum(threadNum),
    m_isExit(false),
    m_taskqueue(queSize)
{
    m_threads.reserve(m_threadNum);
}

Threadpool::~Threadpool()
{
    if(!m_isExit)
    {
        stop();
    }
}

void Threadpool::start()
{
    for(size_t idx = 0; idx != m_threadNum; ++idx)
    {
        unique_ptr<Thread> pthread(new Thread(
            std::bind(&Threadpool::threadFunc, this), idx
        ));
        m_threads.push_back(std::move(pthread));
        //m_threads.push_back(pthread);//error can't copy unique_ptr
    }

    for(auto& pthread : m_threads)
    {
        pthread->create();
    }
}

void Threadpool::threadFunc()
{
    while(!m_isExit)
    {
        Task task = getTask();
        if(task)
        {
            task();
        }
    }
}


void Threadpool::stop()
{
    if(!m_isExit)
    {
        while(!m_taskqueue.empty())
        {
            ::sleep(1);
        }

        m_isExit = true;
        m_taskqueue.wakeup();
        for(auto & pthread : m_threads)
        {
            pthread->join();
        }

    }
}

void Threadpool::addTask(Task &&task)
{
    m_taskqueue.push(task);
}

Task Threadpool::getTask()
{
    return m_taskqueue.pop();
}














}//wd
