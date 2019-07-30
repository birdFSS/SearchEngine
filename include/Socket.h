#pragma once

namespace wd
{

class Socket
{
public:
    Socket();
    explicit Socket(int fd);

    int getFd()  const { return m_fd; }

    void shutdownWrite();
    ~Socket();
private:
    int m_fd;
};







}//end of namespace wd

