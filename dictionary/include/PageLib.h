#pragma once
#include "DirScanner.h"
#include "Configuration.h"
#include "RssReader.h"
#include <vector>
#include <map>
#include <string>

namespace wd
{

class PageLib
{
public:
    PageLib(Configuration& conf, DirScanner& scan) :
        m_conf(conf),
        m_scan(scan)
    {}
    ~PageLib() {}

    void create(); //创建网页库
    void store(); //储存网页库
private:
    Configuration&                      m_conf;
    DirScanner&                         m_scan;
    RssReader                           m_reader;   //存放格式化后的网页
    std::map<int, std::pair<int,int>>   m_offsetLib;

};







}//end of namespace wd

