#pragma once
#include "InetAddress.h"
#include "Socket.h"

namespace wd
{

class Acceptor
{
public:
    Acceptor(unsigned short port);
    Acceptor(const string& ip, unsigned short port);
    void ready();
    int getFd() const;
    int accept();
    ~Acceptor() {}
private:
    void setReuseAddr(bool on);
    void setReusePort(bool on);
    void bind();
    void listen();
private:
    InetAddress m_addr;
    Socket m_listensock;
};







}//end of namespace wd

