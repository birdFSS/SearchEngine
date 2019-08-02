#include "../include/Mylog.h"
#include <time.h>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/Priority.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/OstreamAppender.hh>
#include <string>
using std::string;
using std::to_string;
namespace wd
{


MyLogger* MyLogger::m_myLogger = nullptr;

MyLogger::MyLogger()
    :m_root(log4cpp::Category::getRoot())
{
    log4cpp::Appender *appender1 = new log4cpp::OstreamAppender("console",&std::cout);
    /*为什么输出到文件的信息也会出现在标准输入*/
    //按天标记日志名，方便查询
#if 1
    time_t time_val = time(nullptr);
    struct tm ret;
    gmtime_r(&time_val, &ret);
    char buf[1024] = {0};
    sprintf(buf, "%s%04d%02d%02d%s", "../log/",ret.tm_year+1900, ret.tm_mon + 1, ret.tm_mday, "Spell.log");
#endif

    log4cpp::Appender *appenderFile = new log4cpp::FileAppender("default",buf);
    
    /*设置格式1*/
    log4cpp::PatternLayout* ptnLayout1 = new log4cpp::PatternLayout();
    ptnLayout1->setConversionPattern("%d [%p] %m%n");
    appender1->setLayout(ptnLayout1);
    /*设置格式2*/
    log4cpp::PatternLayout* ptnLayout2 = new log4cpp::PatternLayout();
    ptnLayout2->setConversionPattern("%d [%p] %m%n");
    appenderFile->setLayout(ptnLayout2);

    m_root.addAppender(appender1);
    m_root.addAppender(appenderFile);
    //优先级以下的不会提示
    m_root.setPriority(log4cpp::Priority::DEBUG);

}

MyLogger* MyLogger::getInstance()
{
    if(m_myLogger == nullptr)
    {
        m_myLogger = new MyLogger();
    }
    return m_myLogger;
}

void MyLogger::destroy()
{
    if(m_myLogger != nullptr)
    {
        m_myLogger->m_root.shutdown();
        delete m_myLogger;
        m_myLogger = nullptr;
    }
}



}//wd
