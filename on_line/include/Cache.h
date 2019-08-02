#pragma once
#include <unordered_map>
#include <list>
#include <iostream>
#include <vector>
#define CACHE_SIZE 10


#define CACHE_DEBUG 0

namespace wd
{


class Cache
{
    friend class MyTask;
    struct CacheNode
    {
        CacheNode(const std::string& key,const std::string value) :
            m_key(key),
            m_value(value)
        {}
        std::string m_key;
        std::string m_value;
    };
public:
    Cache();
    Cache(const Cache& cache);
    ~Cache();
    void addElement(const std::string& key, const std::string &value);
    void readFromFile(const std::string& fileName);
    void writeToFile(const std::string& fileName);
    void update(const Cache& rhs);
    static void getMostFrequentlyUsedData(std::vector<Cache>& Caches);   //将其他缓冲信息统一到一个list

    void showList() const       //test
    {
        std::cout << "--------list-----------------------" << std::endl;
        for(auto& node : m_list)
        {
            std::cout << node.m_key << "-->" << node.m_value << std::endl;
        }
    }

private:
    std::list<CacheNode> m_list;
    std::unordered_map<std::string, std::list<CacheNode>::iterator> m_hashMap;
};







}//end of namespace wd

