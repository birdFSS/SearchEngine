#include "../include/WordQueryServer.h"
#include "../include/MyRedis.h"
#include "../include/TimerThread.h"
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
    string msg(conn->receive());       
    size_t pos = msg.find('\n');
    msg = msg.substr(0, pos);
    logInfo(">> receive msg from client : %s", msg.c_str());
    
    m_threadpool.addTask(
        std::bind(&WordQueryServer::doTaskThread, this, conn, msg)
        );
}


void WordQueryServer::doTaskThread(const TcpConnectionPtr& conn, const string& msg)
{
    MyRedis* cache = MyRedis::getInstance();
    string message =cache->get(msg.c_str());
    //cache->set("321", "15\n nihao \n who");//可以成功
    if(message == string("-1"))
    {

        string ret = m_wordQuery.doQuery(msg);
        int sz = ret.size();
        string mess(std::to_string(sz));
        mess.append("\n").append(ret);
        cache->set(msg, mess);
        conn->sendInLoop(mess);
    }else{
        conn->sendInLoop(message);
    }
}

void WordQueryServer::start()
{
    //使用redis不用自身的Cache了
#if 0
    auto& config = m_conf.getConfigMap();
    //初始化缓存,可以考虑设置一个标记位，判断是否init过，避免一些情况
    auto pCacheMana = CacheManager::getInstance();
    pCacheMana->initCache(stoi(config.at("threadNum")), config.at("cacheFilePath"));
#endif
    m_threadpool.start();
#if 0
    TimerThread timerThread(stoi(config.at("initTime")), 
                            stoi(config.at("intervalTime")),
                            std::bind(&CacheManager::periodicUpdateCaches, pCacheMana));
    timerThread.start();
#endif
    using namespace std::placeholders;
    m_tcpServer.setConnectionCallBack(std::bind(&WordQueryServer::onConnection, this, _1));
    m_tcpServer.setMessageCallBack(std::bind(&WordQueryServer::onMessage, this, _1));
    m_tcpServer.setCloseCallBack(std::bind(&WordQueryServer::onClose, this, _1));
    

    m_tcpServer.start();

}

















}//wd

