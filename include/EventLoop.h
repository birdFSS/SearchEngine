#pragma once
#include "Mutexlock.h"
#include <sys/epoll.h>
#include <map>
#include <memory>
#include <vector>
#include <functional>

namespace wd
{
class Acceptor;
class TcpConnection;

class EventLoop
{
public:
    using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
    using TcpConnectionCallBack = std::function<void(const TcpConnectionPtr&)>;
    using Functor = std::function<void()>;
    EventLoop(Acceptor &);
    void loop();
    void unloop();
    void runInLoop(Functor && cb);

    void setConnectionCallBack(TcpConnectionCallBack && cb)
    { m_onConnection = std::move(cb); }
    void setMessageCallBack(TcpConnectionCallBack && cb)
    { m_onMessage= std::move(cb); }
    void setCloseCallBack(TcpConnectionCallBack && cb)
    { m_onClose= std::move(cb); }
    
private:
    void waitEpollFd();
    void handleNewConnection();
    void handleMessage(int fd);
    
    
    void handleRead();
    void wakeup();
    int createEventFd();
    void doPendingFunctors();
    int createEpollFd();
    
    void addEpollFdRead(int fd);
    void delEpollFdRead(int fd);
    bool isConnectionClosed(int fd);
private:
    int m_efd;
    int m_eventfd;
    Acceptor & m_acceptor;
    std::vector<struct epoll_event> m_eventList;
    std::map<int, TcpConnectionPtr> m_conns;
    bool m_isLooping;

    MutexLock m_mutex;
    std::vector<Functor> m_pendingFunctors;     //需要延迟执行的回调函数
    //整体处理的回调函数
    TcpConnectionCallBack m_onConnection;
    TcpConnectionCallBack m_onMessage;
    TcpConnectionCallBack m_onClose;

};







}//end of namespace wd

