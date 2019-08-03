#include "ClientIO.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
using std::string;
namespace wd
{

string ClientIO::recvFromServer()
{
#if 0
    Train train;
    readn(reinterpret_cast<char*>(&train), TRAIN_HEAD_SIZE);
    readn(train._buf, train._size - TRAIN_HEAD_SIZE);
#endif
    int _size = 0;
    readn((char*)&_size, sizeof(int));
    char * buf = new char[_size - TRAIN_HEAD_SIZE]();
    readn(buf, _size - TRAIN_HEAD_SIZE);
    string tmpBuf(buf);
    delete [] buf;
    return tmpBuf;
}

void ClientIO::sendToServer(const char *msg)
{
    if(nullptr == msg || strlen(msg) == 0)
    {
        return;
    }
    Train train;
    ::bzero(&train, sizeof(Train));
    train._size = strlen(msg) + TRAIN_HEAD_SIZE;
    ::strcpy(train._buf, msg);
    //printf("send info :$%s$", msg);
    writen(reinterpret_cast<char*>(&train), train._size);
}

ssize_t ClientIO::readn(char *buff, size_t len)
{
    char *p = buff;
    size_t totRead;
    ssize_t numRead;
    for(totRead = 0; totRead < len;)
    {
        numRead = ::read(m_fd, p, len - totRead);
        if(-1 == numRead && EINTR == errno) //Interrupted  --> restart read()
        {
            if(EINTR == errno)
            {
                continue;
            }else{
                perror("read");
                return totRead;
            }
        }else if(0 == numRead){         //对端断开，numRead 为 0
            return totRead;
        }else{
            totRead += numRead;
            p += numRead;
        }
    }
    return totRead;
}


ssize_t ClientIO::writen(const char* buff,size_t len)
{
    size_t totWrite = 0;
    const char* p = buff;
    while(totWrite < len)
    {
        ssize_t numWrite = ::write(m_fd, p, len - totWrite);
        if(numWrite == -1 && errno == EINTR) //Interrupted  --> restart read()
        {
            continue;
        }else if(-1 == numWrite){
            perror("write");
            return totWrite;
        }else{
            totWrite += numWrite;
            p += numWrite;
        }
    }
    return totWrite;
}

















}




















