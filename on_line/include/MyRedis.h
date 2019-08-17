#pragma once
#include "Mylog.h"
#include <hiredis/hiredis.h>
#include <stdlib.h>
#include <string.h>

#include <string>
using std::string;
namespace wd
{

class MyRedis
{
public:
    static MyRedis* getInstance()
    {
        if(nullptr == m_self)
        {
            m_self = new MyRedis();
        }
        return m_self;
    }

    void set(const string& key,const string& value)
    {
#if 0
        logInfo("set(const char* key, const char* value)");
        logInfo("key=%s,value=%s\n-----------", key.c_str(), value.c_str());
        logInfo("strlen(value)=%ld", strlen(value.c_str()));
#endif
        redisReply * reply = (redisReply*)redisCommand(m_redisConn,"SET %s %s", key.c_str(), value.c_str()); 
        if(nullptr == reply)
        {
            logError("command execute failure:%s", key);
            //redisFree(m_redisConn);
            return;
        }

        if(!(REDIS_REPLY_STATUS == reply->type && strcmp(reply->str, "OK") == 0))
        {
            logError("command execute failure\n");
            freeReplyObject(reply);
            return;
        }
        freeReplyObject(reply);
    }

    string get(const char* key)
    {
        string getVal = "GET ";
        getVal.append(key);
        redisReply * reply = (redisReply*)redisCommand(m_redisConn, getVal.c_str());
        if(nullptr == reply || reply->type != REDIS_REPLY_STRING)
        {
            logError("command execute failure:%s\n", getVal.c_str());
            freeReplyObject(reply);
            //redisFree(m_redisConn);   //之前这里错误直接断开连接，导致了bug
            return string("-1");
        }
        string tmp(reply->str);
        freeReplyObject(reply);
        return tmp;
    }

private:
    MyRedis() :
        m_redisConn(redisConnect("127.0.0.1", 6379))
    {
        if(m_redisConn->err)
        {
            logError("error build redis connect");
            redisFree(m_redisConn);
            exit(0); 
        }
    }
    ~MyRedis() {}
    
private:
    static MyRedis* m_self;
    redisContext*   m_redisConn;
};







}//end of namespace wd

