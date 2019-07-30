#pragma once
#include "Acceptor.h"
#include "EventLoop.h"
#include "TcpConnection.h"

namespace wd
{

using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
using TcpConnectionCallBack = std::function<void(const TcpConnectionPtr&)>;

class TcpServer
{
public:
    TcpServer(const string& ip, unsigned short port);
    void start();

    void setConnectionCallBack(TcpConnectionCallBack && cb);
    void setMessageCallBack(TcpConnectionCallBack && cb);
    void setCloseCallBack(TcpConnectionCallBack && cb);

private:
    Acceptor m_acceptor;
    EventLoop m_loop;
};







}//end of namespace wd

