#pragma once
#include "Configuration.h"
#include "TimerThread.h"
#include "CacheManager.h"
#include "TcpServer.h"
#include "Threadpool.h"
#include "WordQuery.h"

#include <string>

namespace wd
{

class WordQueryServer
{
public:
    WordQueryServer(const string& fileName) :
        m_conf(fileName),
        m_wordQuery(m_conf),
        m_tcpServer(
                m_conf.getConfigMap()["ip"],
                stoi( m_conf.getConfigMap()["port"])
                ),
        m_threadpool(
                stoi(m_conf.getConfigMap()["threadNum"]),
                stoi(m_conf.getConfigMap()["queueSize"])
                )
    {

    }
    ~WordQueryServer() {}

    void start();
private:
    void onConnection(const TcpConnectionPtr& conn);
    void onMessage(const TcpConnectionPtr& conn);
    void onClose(const TcpConnectionPtr& conn);
    void timerFunc();
    void doTaskThread(const TcpConnectionPtr & conn, const string& msg);
private:
    Configuration   m_conf;    
    WordQuery       m_wordQuery;
    TcpServer       m_tcpServer;
    Threadpool      m_threadpool;
};







}//end of namespace wd

