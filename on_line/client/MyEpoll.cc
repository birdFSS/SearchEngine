#include "MyEpoll.h"
#include "Client.h"

#include <json/json.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
using std::cout;
using std::endl;


namespace wd
{

MyEpoll::MyEpoll(Client & client) :
    m_efd(createEpollFd()),
    m_isLooping(false),
    m_client(client),
    m_clientIO(m_client.getFd()),
    m_eventList(2)
{
    addEpollFdRead(STDIN_FILENO);
    addEpollFdRead(m_client.getFd());
}

int MyEpoll::createEpollFd()
{
   int ret = ::epoll_create1(0);    //epoll_create(0) 是错误的
   if(-1 == ret)
   {
       perror("epoll_create1");
   }
   return ret;
}


void MyEpoll::addEpollFdRead(int fd)
{
    struct epoll_event evt;
    evt.data.fd = fd;
    evt.events = EPOLLIN;
    int ret = epoll_ctl(m_efd, EPOLL_CTL_ADD, fd, &evt);
    if(-1 == ret)
    {
        perror("epoll_ctl");
    }
}

void MyEpoll::waitEpollFd()
{
    int readyNum;
    do{
        readyNum = ::epoll_wait(m_efd, &*m_eventList.begin(), m_eventList.size(), -1);
    }while(-1 == readyNum && EINTR == errno);

    if(-1 == readyNum)
    {
        perror("epoll_wait");
        ::exit(-1);
    }else if(0 == readyNum){
        cout << "timeout" << endl;
    }else{
#if 1
        if(readyNum == static_cast<int>(m_eventList.size()))
        {
            m_eventList.resize(2 * readyNum);
        }
#endif
        for(int idx = 0; idx != readyNum; ++idx)
        {
            int fd = m_eventList[idx].data.fd;
            if(fd == STDIN_FILENO)
            {
                char buff[65536] = {0};
                ::read(fd, buff, sizeof(buff));
                //buff[strlen(buff) - 1] = '\0';    //服务器原因需要传\n过去
                //::write(m_client.getFd(), buff, strlen(buff));
                buff[strlen(buff) - 1] = '\0';      //修改后，服务器不用readline，这边也要去掉'\n'
                m_clientIO.sendToServer(buff);
            }

            if(fd == m_client.getFd())
            {
                std::string str= m_clientIO.recvFromServer();
                Json::Reader reader;
                Json::StyledWriter styled_writer;
                Json::Value val;
                if(!reader.parse(str, val))
                {
                    perror(">>json parse error");
                    exit(-1);
                }
                cout << styled_writer.write(val) << endl;
            }
        }
    }

}

void MyEpoll::loop()
{
    m_isLooping = true;
    while(m_isLooping)
    {
        waitEpollFd();
    }
}

void MyEpoll::unloop()
{
    m_isLooping = false;
}










}//wd















