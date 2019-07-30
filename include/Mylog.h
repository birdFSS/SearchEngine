#pragma once
#include <log4cpp/Category.hh>                                             
#if 0       //添加到.cc文件中了
#include <log4cpp/Appender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/Priority.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/Layout.hh>
#include <log4cpp/BasicLayout.hh>
#endif

namespace wd
{



class MyLogger
{
public:
    template <class... Args>
    void warn(Args... args)
    {
         m_root.warn(args...);
    }

    template <class... Args>
    void error(Args... args)
    {
         m_root.error(args...);
    }

    template <class... Args>
    void debug(Args... args)
    {
         m_root.debug(args...);
    }

    template <class... Args>
    void info(Args... args)
    {
         m_root.info(args...);
    }

    static MyLogger* getInstance();
    static void destroy();
private:
    MyLogger();
    ~MyLogger() {}
private:  
    log4cpp::Category& m_root;
    static MyLogger* m_myLogger;
};



}//wd

//定义一个在日志后添加 文件名 函数名 行号 的宏定义
#define log4cpp_suffix(msg)  std::string("[").append(__FILE__)\
    .append(":").append(__func__)\
    .append(":").append(std::to_string(__LINE__))\
    .append("] ").append(msg).c_str()


//缩短并简化函数调用形式
#define logError(msg, ...) wd::MyLogger::getInstance()->error(log4cpp_suffix(msg), ##__VA_ARGS__)
#define logWarn(msg, ...) wd::MyLogger::getInstance()->warn(log4cpp_suffix(msg), ##__VA_ARGS__)
#define logInfo(msg, ...) wd::MyLogger::getInstance()->info(log4cpp_suffix(msg), ##__VA_ARGS__)
#define logDebug(msg, ...) wd::MyLogger::getInstance()->debug(log4cpp_suffix(msg),  ##__VA_ARGS__)





