#include "../include/SocketIO.h"

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>

//EINTR
//The receive was interrupted by delivery of a signal before any data were available;

namespace wd
{

/*这里采用预读取*/
ssize_t SocketIO::readn(char *buff, size_t len)
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

ssize_t SocketIO::readline(char* buff, size_t maxlen)
{
    size_t left = maxlen - 1;
    char * p = buff;
    ssize_t numRead;
    size_t total = 0;
    while(left > 0)
    {
        numRead = recvPeek(p, left);
        for(ssize_t idx = 0; idx != numRead; ++idx)
        {
            if(p[idx] == '\n')
            {
                ssize_t sz = idx + 1;
                readn(p, sz);
                total += sz;
                p += sz - 1;
                *p = '\0';
                return total;
            }
        }
        //如果没有发现\n
        readn(p, numRead);
        left -= numRead;
        p += numRead;
        total += numRead;
    }
    //达到最大长度还没有发现
    *p = '\0';
    return total;
}

ssize_t SocketIO::writen(const char* buff,size_t len)
{
    size_t totWrite = 0;
    const char* p = buff;
    while(totWrite < len)
    {
        ssize_t numWrite = ::write(m_fd, p, len - totWrite);
        //printf("fd = %d\n msg=%s\nlen - totwtite = %ld\n", m_fd, p, len-totWrite);
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

ssize_t SocketIO::recvPeek(char * buff, size_t len)
{
    ssize_t numRead;
    do{
        numRead = ::recv(m_fd, buff, len, MSG_PEEK); //MSG_PEEK 表示不会删除内核输入缓冲区内容，仅复制到buff缓冲区 
    }while(numRead == -1 && errno == EINTR);
    return numRead;
}


























}//wd
