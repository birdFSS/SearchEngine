#pragma once
#include "Timer.h"
#include "Thread.h"
#include "CacheManager.h"

namespace wd
{

class TimerThread
{
public:
    typedef std::function<void()> TimerCallBack;
    TimerThread(int initSec, int intervalSec, TimerCallBack &&cb) :
        m_timer(initSec, intervalSec, std::move(cb)),
        m_thread(std::bind(&Timer::start, &m_timer))
    {}
    ~TimerThread() {}
    void start()
    {
        m_thread.create();
    }
    void stop()
    {
        m_timer.stop();
        m_thread.join();
    }


    int getFd() const
    { return m_timer.getFd(); }
private:
    Timer m_timer;
    Thread m_thread;
};







}//end of namespace wd

