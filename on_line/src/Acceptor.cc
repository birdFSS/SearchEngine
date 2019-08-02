#include "../include/Acceptor.h"



namespace wd
{

Acceptor::Acceptor(unsigned short port) : m_addr(port), m_listensock()
{
    
}

Acceptor::Acceptor(const string& ip, unsigned short port) :
    m_addr(ip, port),
    m_listensock()
{}

int Acceptor::getFd() const
{
    return m_listensock.getFd();
}

void Acceptor::ready()
{
    setReuseAddr(true);
    setReusePort(true);
    bind();
    listen();
}

void Acceptor::bind()
{
    int ret = ::bind(m_listensock.getFd(), (struct sockaddr *)m_addr.getInetAddressPtr(), m_addr.getSize());
    if(-1 == ret)
    {
        perror("bind");
    }
}

void Acceptor::listen()
{
    int ret = ::listen(m_listensock.getFd(), 10);
    if(-1 == ret) 
    {
        perror("listen");
    }
}

int Acceptor::accept()
{
    int peerFd = ::accept(m_listensock.getFd(),  NULL, NULL);
    if(-1 == peerFd)
    {
        perror("accept");
    }
    return peerFd;
}

//这个不太懂，学习一下
void Acceptor::setReuseAddr(bool on)
{
    int one = on;
    if(::setsockopt(m_listensock.getFd(), SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one)) < 0)
    {
        perror("setsockopt");
    }
}


void Acceptor::setReusePort(bool on)
{
    int one = on;
    if(::setsockopt(m_listensock.getFd(), SOL_SOCKET, SO_REUSEPORT, &one, sizeof(one)) < 0)
    {
        perror("setsockopt");
    }
}

















}//wd
