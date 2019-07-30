#include "../include/EventLoop.h"
#include "../include/Acceptor.h"
#include "../include/TcpConnection.h"
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <sys/eventfd.h>
#include <iostream>


using std::cout;
using std::endl;

namespace wd
{

EventLoop::EventLoop(Acceptor & accp) :
    m_efd(createEpollFd()),
    m_eventfd(createEventFd()),
    m_acceptor(accp),
    m_eventList(1024),
    m_isLooping(false)
{
    addEpollFdRead(m_acceptor.getFd());
    addEpollFdRead(m_eventfd);
    //printf("listen fd = %d\neventfd = %d\n", m_acceptor.getFd(), m_eventfd);
}


void EventLoop::handleRead()
{
    uint64_t howmany;
    int ret = read(m_eventfd, &howmany, sizeof(uint64_t));
    if(ret != sizeof(howmany))
    {
        perror("read");
    }
}

void EventLoop::wakeup()
{
    uint64_t one = 1;
    printf("EventLoop::wakeup()\n");
    int ret = ::write(m_eventfd, &one, sizeof(one));
    if(ret != sizeof(one))
    {
        perror("write");
    }
}

int EventLoop::createEventFd()
{
    int ret = ::eventfd(10,0);  //这里写成(0,0)
    if(ret == -1)
    {
        perror("eventfd");
    }
    return ret;
}

void EventLoop::doPendingFunctors()
{
    std::vector<Functor> tmp;
    {
        MutexLockGuard autolock(m_mutex);
        tmp.swap(m_pendingFunctors);
    }
    //printf("vector<Functor> size = %ld\n",tmp.size() );
    for(auto & functor : tmp)
    {
        functor();
    }
}

void EventLoop::runInLoop(Functor && cb)
{
    //printf("EventLoop::runInLoop(Functor && cb)\n");
    {
        MutexLockGuard autolock(m_mutex);
        m_pendingFunctors.push_back(std::move(cb));
    }
    wakeup();
}

int EventLoop::createEpollFd()
{
    int ret = ::epoll_create1(0);
    if(-1 == ret)
    {
        perror("epoll_create1");
    }
    return ret;
}

//开关接口
void EventLoop::loop()
{
    m_isLooping = true;
    while(m_isLooping)
    {
        waitEpollFd();
    }
}

void EventLoop::unloop()
{
    m_isLooping = false;
}

//实际操作
void EventLoop::waitEpollFd()
{
    int readyNum;
    //printf("go in epoll_wait\n");
    do{
        readyNum = epoll_wait(m_efd, &*m_eventList.begin(), m_eventList.size(), 5000);
    }while(-1 == readyNum && EINTR == errno);

    //printf("out epoll_wait, readyNum = %d \n" , readyNum);
    if(-1 == readyNum)
    {
        perror("epoll_wait");
        return;
    }else if(0 == readyNum){
        cout << "epoll_wait timeout " << endl;
    }else{
        if(readyNum == (int)m_eventList.size())
        {
            m_eventList.resize(2 * readyNum);
        }

        for(int idx = 0; idx != readyNum; ++idx)
        {
            int fd = m_eventList[idx].data.fd;

            if(fd == m_acceptor.getFd()) 
            {
                if(m_eventList[idx].events & EPOLLIN)   //处理新链接
                {
                    handleNewConnection();
                }
            }else if(fd == m_eventfd){
                if(m_eventList[idx].events & EPOLLIN)
                {
                    handleRead();
                    cout << ">> before doPendingFunctors()" << endl;
                    doPendingFunctors();
                    cout << ">> after doPendingFunctors()" << endl;
                }
            }else{
                if(m_eventList[idx].events & EPOLLIN) //处理消息
                {
                    handleMessage(fd);
                }
            }
        }
    }
}

void EventLoop::handleNewConnection()
{
    int peerFd = m_acceptor.accept();
    printf("new connect fd = %d\n", peerFd);
    addEpollFdRead(peerFd);
    TcpConnectionPtr conn(new TcpConnection(peerFd, this));
    //回调函数设置
    conn->setConnectionCallBack(m_onConnection);
    conn->setCloseCallBack(m_onClose);
    conn->setMessageCallBack(m_onMessage);
    m_conns.insert(std::make_pair(peerFd, conn));
    
    conn->handleConnectionCallBack();
}

void EventLoop::handleMessage(int fd)
{
    bool isClosed = isConnectionClosed(fd);
    auto iter = m_conns.find(fd);
    assert(iter != m_conns.end());

    if(!isClosed)
    {
        iter->second->handleMessageCallBack();        
    }else{
        delEpollFdRead(fd);
        iter->second->handleCloseCallBask();
        m_conns.erase(iter);
    }
}

void EventLoop::addEpollFdRead(int fd)
{
    struct epoll_event evt;
    evt.data.fd = fd;
    evt.events = EPOLLIN;
    if(epoll_ctl(m_efd, EPOLL_CTL_ADD, fd, &evt) == -1)
    {
        perror("epoll_ctl");
    }
}

void EventLoop::delEpollFdRead(int fd)
{
    struct epoll_event evt;
    evt.data.fd = fd;
    if(epoll_ctl(m_efd, EPOLL_CTL_DEL, fd, &evt) == -1)
    {
        perror("epoll_ctl");
    }
}

bool EventLoop::isConnectionClosed(int fd)
{
    int ret;
    do{
        char buf[1024];
        ret = recv(fd, buf, 1024, MSG_PEEK);
    }while(-1 == ret && EINTR == errno);

    return ret == 0;
}







}//wd
