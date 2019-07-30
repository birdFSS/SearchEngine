#pragma once
#include <fstream>
#include <string>
#include <map>

namespace wd
{

class Configuration
{
public:
    Configuration(const std::string& fileName) : m_fileName(fileName) 
    {
        std::ifstream ifs(m_fileName);
        std::string name, data;
        while(ifs >> name >> data)
        {
            m_configMap.insert(std::make_pair(name, data));
        }
    }
    ~Configuration() {}
    std::map<std::string, std::string> &
    getConfigMap();
private:
    std::string m_fileName;
    std::map<std::string, std::string> m_configMap;
};

std::map<std::string, std::string> &
Configuration::getConfigMap()
{
    return m_configMap;
}






}//end of namespace wd

