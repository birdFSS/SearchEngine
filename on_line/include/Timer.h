#pragma once

#include <sys/timerfd.h>
#include <functional>
namespace wd
{

using TimerCallBack = std::function<void()>;
class Timer
{
public:
    Timer(int initSec, int intervalSec, TimerCallBack && cb);
    ~Timer();
    void start();
    void stop();
    int getFd() const
    { return m_timerfd; }
    void handleRead();
    void runCallBack()
    {
        if(m_isStarted && m_cb)
        {
            m_cb();
        }
    }
private:

    int createTimerfd();
    void setTimer(int initSec, int intervalSec);

private:
    int m_timerfd;
    bool m_isStarted;
    struct itimerspec m_ts;
    TimerCallBack m_cb;
};







}//end of namespace wd

