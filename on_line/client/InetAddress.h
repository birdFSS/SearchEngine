#pragma once
/* 负责生成sockaddr_in结构体，
 * 读取ip地址和端口号
 * 
 * */
#include <arpa/inet.h>
#include <string>
using std::string;

namespace wd
{

class InetAddress
{
public:
    explicit
    InetAddress(unsigned short port);
    InetAddress(const string& ip, unsigned short port);
    InetAddress(const struct sockaddr_in & addr);
    ~InetAddress() {}

    string getIp() const;
    unsigned short getPort() const;
    struct sockaddr_in * getInetAddressPtr() { return &m_addr; }
    int getSize() const { return sizeof(struct sockaddr_in); }

private:
    struct sockaddr_in m_addr;
};







}//end of namespace wd

