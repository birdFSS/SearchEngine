#include "../include/TaskQueue.h"
#include <iostream>

using std::cout;
using std::endl;

namespace wd
{

TaskQueue::TaskQueue(size_t queSize) :
    m_queSize(queSize),
    m_isOpen(true),
    m_mutex(),
    m_notFull(m_mutex),
    m_notEmpty(m_mutex)
{

}

void TaskQueue::wakeup()
{
    m_isOpen = false;
    m_notFull.notifyAll();
    m_notEmpty.notifyAll();
}

bool TaskQueue::empty()  const 
{
    return m_que.size() == 0;
}

bool TaskQueue::full() const 
{
    return m_que.size() == m_queSize;
}

void TaskQueue::push(Task val)
{
    {
        MutexLockGuard autolock(m_mutex);
        while(m_isOpen && full())
        {
            m_notFull.wait();
        }

        m_que.push(val);
    }
    m_notEmpty.notify();
}

Task TaskQueue::pop()
{
    Task val;
    {
        MutexLockGuard autolock(m_mutex);
        while(m_isOpen && empty())
        {
            m_notEmpty.wait();
        }

        if(m_isOpen){
            val = m_que.front();
            m_que.pop();
        }else{
            return nullptr;
        }
    }
    m_notFull.notify();//老师的放在里面
    return val;
}












}//wd































