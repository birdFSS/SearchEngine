#pragma once
#include <stdio.h>
namespace wd
{

class SocketIO
{
public:
    explicit
    SocketIO(int fd) : m_fd(fd){}
    
    ssize_t readn(char *buff, size_t len);
    ssize_t readline(char* buff, size_t maxlen);
    ssize_t writen(const char* buff, size_t len);
private:
    ssize_t recvPeek(char* buff, size_t len);
private:
    int m_fd;
};







}//end of namespace wd

