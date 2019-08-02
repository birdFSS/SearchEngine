#pragma once
#include "InetAddress.h"
namespace wd
{


//暂时先这样，后面再重构一下，可以MVC设计模式
class Client
{
public:
    Client(const string& ip, unsigned short port) :
        m_fd(0),
        m_addr(ip, port)
    {}
    ~Client() {}

    void run();
    int getFd() const
    { return m_fd; }

private:
    void connectToServer();
private:
    int m_fd;
    InetAddress m_addr;
};







}//end of namespace wd

