#pragma once
#include "Mylog.h"
#include <fstream>
#include <string>
#include <set>
#include <map>

namespace wd
{

class Configuration
{
public:
    Configuration(const std::string& fileName) : 
        m_fileName(getAbosolutePath(fileName)) 
    {
        std::ifstream ifs(m_fileName);
        std::string name, data;
        while(ifs >> name >> data)
        {
            m_configMap.insert(std::make_pair(name, data));
        }
        
        ifs.close();

        auto iter = m_configMap.find("stopWords");
        if(iter != m_configMap.end())
        {
            ifs.open(iter->second);
            std::string word;
            while(ifs >> word)
            {
                m_stopWords.insert(word);
            }
        }
        //删除空格和换行和tab
        m_stopWords.insert(std::string(" "));
        std::string full_space("\xc2\xa0");
        m_stopWords.insert(full_space);

        //logError("$%s$", full_space.c_str());
        m_stopWords.insert(std::string("\n"));
        m_stopWords.insert(std::string("\t"));
    }

    ~Configuration() {}

    std::map<std::string, std::string> &  getConfigMap()
    {
        return m_configMap;
    }

    std::set<std::string> & getStopWords()
    {
        return m_stopWords;
    }

private:
    std::string getAbosolutePath(const std::string & fileName)
    {
        char buff[2048] = {0};
        if(realpath(fileName.c_str(), buff))
        {
            return std::string(buff);
        }else{
            logError("realpath");
            return std::string("");
        }   
    }
    
private:
    std::string m_fileName;
    std::set<std::string> m_stopWords;
    std::map<std::string, std::string> m_configMap;
};







}//end of namespace wd

