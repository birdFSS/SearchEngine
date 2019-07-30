#pragma once
#include "MyConf.h"
#include "TimerThread.h"
#include "CacheManager.h"
#include "TcpServer.h"
#include "Threadpool.h"

#include <string>
namespace wd
{

class SpellcorrectServer
{
public:
    SpellcorrectServer(MyConf &conf) :
        m_conf(conf),
        m_tcpServer(
                m_conf.getConfig()["ip"],
                stoi( m_conf.getConfig()["port"])
                ),
        m_threadpool(
                stoi(m_conf.getConfig()["threadNum"]),
                stoi(m_conf.getConfig()["queueSize"])
                )
    {

    }
    ~SpellcorrectServer() {}

    void start();
    void onConnection(const TcpConnectionPtr& conn);
    void onMessage(const TcpConnectionPtr& conn);
    void onClose(const TcpConnectionPtr& conn);
    void timerFunc();

private:
    MyConf& m_conf;    
    TcpServer m_tcpServer;
    Threadpool m_threadpool;
};







}//end of namespace wd

