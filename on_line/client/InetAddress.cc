#include "InetAddress.h"
#include <string.h>

namespace wd
{

InetAddress::InetAddress(unsigned short port)
{
    ::memset(&m_addr, 0, sizeof(struct sockaddr_in));
    m_addr.sin_port = ::htons(port);        //主机字节序转网络字节序
    m_addr.sin_family = AF_INET;
    m_addr.sin_addr.s_addr = INADDR_ANY;    //inet_addr(0,0,0,0) 表示本机地址
}

InetAddress::InetAddress(const string& ip, unsigned short port)
{
    ::memset(&m_addr, 0, sizeof(struct sockaddr_in));
    m_addr.sin_family = AF_INET;
    m_addr.sin_port = ::htons(port);
    m_addr.sin_addr.s_addr = ::inet_addr(ip.c_str());
}

InetAddress::InetAddress(const struct sockaddr_in & addr)
    : m_addr(addr)
{
    
}

string InetAddress::getIp() const
{
    return string(::inet_ntoa(m_addr.sin_addr));
}

unsigned short InetAddress::getPort() const 
{
    return ::ntohs(m_addr.sin_port);
}


















}//wd
