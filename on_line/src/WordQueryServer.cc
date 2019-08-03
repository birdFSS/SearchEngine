#include "../include/WordQueryServer.h"
#include "../include/CacheManager.h"
#include "../include/TimerThread.h"
#include "../include/MyTask.h"
#include "../include/Mylog.h"
#include <string>
#include <iostream>
#include <string>
#include <functional>

using std::cout;
using std::endl;
namespace wd
{

void WordQueryServer::onConnection(const wd::TcpConnectionPtr & conn)
{
    logInfo("%s has connected!", conn->toString().c_str());
}

void WordQueryServer::onClose(const wd::TcpConnectionPtr & conn)
{
    logInfo(">> %s has closed!", conn->toString().c_str());
}

void WordQueryServer::onMessage(const wd::TcpConnectionPtr & conn)
{
    //该回调函数实际不宜过长 2ms
    //cout << "onMessage..." << endl;
    string msg = conn->receive();       
    logInfo(">> receive msg from client : %s", msg.c_str());
    
    wd::MyTask task(msg,conn->getPeerFd(),conn);
    m_threadpool.addTask(
        std::bind(&wd::MyTask::excute, task)
        );
}


void WordQueryServer::start()
{
    auto& config = m_conf.getConfig();
    
    //初始化缓存,可以考虑设置一个标记位，判断是否init过，避免一些情况
    auto pCacheMana = CacheManager::getInstance();
    pCacheMana->initCache(stoi(config.at("threadNum")), config.at("cacheFilePath"));

    m_threadpool.start();
    TimerThread timerThread(stoi(config.at("initTime")), 
                            stoi(config.at("intervalTime")),
                            std::bind(&CacheManager::periodicUpdateCaches, pCacheMana));
    timerThread.start();

    using namespace std::placeholders;
    m_tcpServer.setConnectionCallBack(std::bind(&WordQueryServer::onConnection, this, _1));
    m_tcpServer.setMessageCallBack(std::bind(&WordQueryServer::onMessage, this, _1));
    m_tcpServer.setCloseCallBack(std::bind(&WordQueryServer::onClose, this, _1));
    

    m_tcpServer.start();

}

















}//wd

