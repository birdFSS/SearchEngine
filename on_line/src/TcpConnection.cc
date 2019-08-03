#include "../include/TcpConnection.h"
#include "../include/EventLoop.h"
#include <string.h>
#include <sys/socket.h>
#include <iostream>
#include <sstream>

using std::cout;
using std::endl;

namespace wd
{

TcpConnection::TcpConnection(int fd, EventLoop* pLoop) :
    m_sock(fd),
    m_socketIO(fd),
    m_localAddr(getLocalAddr()),
    m_peerAddr(getPeerAddr()),
    m_isShutdownWrite(false),
    m_loop(pLoop)
{

}

TcpConnection::~TcpConnection()
{
    if(!m_isShutdownWrite)
    {
        m_isShutdownWrite = true;
        m_sock.shutdownWrite();
    }
}

void TcpConnection::sendInLoop(const string& msg)
{
    m_loop->runInLoop(std::bind(&TcpConnection::send, this, msg));
}

string TcpConnection::receive()
{
    char buf[1024] = {0};
    size_t ret = m_socketIO.readline(buf, sizeof(buf));  //修改后
    if(0 == ret)
    {
        return std::string();
    }else{
        return string(buf);
    }
}

string TcpConnection::toString() const
{
    std::ostringstream oss;
    oss << m_localAddr.getIp() << ":"<< m_localAddr.getPort() << "--->"
    << m_peerAddr.getIp() << ":" << m_peerAddr.getPort();
    return oss.str();
}

void TcpConnection::send(const string& msg) //当数据过大怎么处理
{
    m_socketIO.writen(msg.c_str(), msg.size()); 
}

void TcpConnection::shutdown()
{
    if(!m_isShutdownWrite)
    {
        m_isShutdownWrite = true;
        m_sock.shutdownWrite();
    }
}

InetAddress TcpConnection::getLocalAddr()
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    if(::getsockname(m_sock.getFd(), (struct sockaddr*)&addr, &len) == -1)
    {
        perror("getsockname");
    }
    return InetAddress(addr);
}


InetAddress TcpConnection::getPeerAddr()
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    if(::getpeername(m_sock.getFd(), (struct sockaddr*)&addr, &len) == -1)
    {
        perror("getpeername");
    }
    return InetAddress(addr);

}

void TcpConnection::handleConnectionCallBack()
{
    if(m_onConnection)
    {
        m_onConnection(shared_from_this());
    }
}

void TcpConnection::handleMessageCallBack()
{
    if(m_onMessage)
    {
        m_onMessage(shared_from_this());
    }
}
void TcpConnection::handleCloseCallBask()
{
    if(m_onClose)
    {
        m_onClose(shared_from_this());
    }
}





}//wd
