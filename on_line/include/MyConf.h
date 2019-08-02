#pragma once
#include <map>
#include <iostream>
#include <fstream>

namespace wd
{

class MyConf
{
public:
    static MyConf* getInstance(const std::string& fileName = "../conf/config.utf8")
    {
        if(m_self == nullptr)
        {
            m_self = new MyConf(fileName);
            atexit(destroy);
        }
        return m_self;
    }

    static void destroy()
    {
        if(m_self)
        {
            delete m_self;
        }
    }

    std::map<std::string, std::string>&
    getConfig()
    { return m_conf; }

    void showConfig() const
    {
        for(auto& i : m_conf)
        {
            std::cout << i.first << "-->" << i.second << std::endl;
        }
    }

private:
    MyConf(const std::string& fileName)
    {
        std::ifstream ifs(fileName);
        std::string name, info;
        while(ifs >> name >> info)
        {
            m_conf.insert(std::make_pair(name,info));
        }
    }
    ~MyConf() {}

private:
    static MyConf* m_self;
    std::map<std::string, std::string> m_conf;
};







}//end of namespace wd

