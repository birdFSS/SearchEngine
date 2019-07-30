#pragma once
#include "InetAddress.h"
#include <string>
#define TRAIN_BUF_SIZE 2048
#define TRAIN_HEAD_SIZE 4

namespace wd
{


class ClientIO
{
public:
    ClientIO(int fd):
        m_fd(fd) 
    {}
    ~ClientIO() {}

    std::string recvFromServer();
    void sendToServer(const char *msg);

    ssize_t readn(char* buff, size_t size);
    ssize_t writen(const char* buff, size_t size);
    struct Train
    {
        int _size;
        char _buf[TRAIN_BUF_SIZE];
    };

private:
    struct train
    {
        int _size;
        char _buf[TRAIN_BUF_SIZE]; 
    };
private:
    int m_fd;
};







}//end of namespace wd

