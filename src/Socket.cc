#include "../include/Socket.h"
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <cstdio>
#include <error.h>

namespace wd
{

Socket::Socket()
{
    m_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == m_fd)
    {
        perror("socket");
    }
}

Socket::Socket(int fd)
{
    m_fd = fd;
}

void Socket::shutdownWrite()
{
    ::shutdown(m_fd, SHUT_WR);
}

Socket::~Socket()
{
    ::close(m_fd);
}


















}










