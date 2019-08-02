#include "MyEpoll.h"
#include "Client.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int recvCycle(int sfd, void* buf, int fileSize){
    int total = 0, ret;
    char* p = (char*)buf;
    while(total < fileSize){
        ret = recv(sfd, p+total, fileSize-total, 0);
        printf("recv ret = %d\n", ret);
        if(0 == ret)
        {
            return -1;
            break;
        }
        total += ret;
    }
    return 0;
}

int main()
{
    wd::Client client("192.168.4.194", 8888);
    client.run();

    wd::MyEpoll epoll(client);
    epoll.loop();
    return 0;
}



int test1()
{
    wd::Client client("192.168.4.194", 8888);
    client.run();
    char buff[65536] = {0};
    ::bzero(buff, strlen(buff));
    int ret;

    strcpy(buff, "apple\n");
    printf("fd = %d\n", client.getFd());
    ret = ::write(client.getFd(), buff, strlen(buff));
    if(-1 == ret)
    {
        perror("write");
        return -1;
    }
    printf("send %s\n", buff);


    ::bzero(buff, strlen(buff));
    printf("fd = %d\n", client.getFd());
    ret =::recv(client.getFd(), buff, sizeof(buff), 0);
    printf("ret = %d\n", ret);
    if(-1 == ret)
    {
        perror("read");
        return -1;
    }
    printf(">> info = %s\n", buff);
    return 0;
}
