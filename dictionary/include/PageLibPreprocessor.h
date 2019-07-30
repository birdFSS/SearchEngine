#pragma once
#include "CppJieba.h"
#include "WebPage.h"
#include <vector>
#include <unordered_map>

namespace wd
{

class Configuration;

class PageLibPreprocessor
{
public:
    PageLibPreprocessor(Configuration& conf);
    ~PageLibPreprocessor() {}

    void doProcess();


private:
    void readInfoFromFile();
    void cutRedundantPages();
    void buildInvertIndexTable();
    void storeOnDisk();

private:
    Configuration&          m_conf;
    CppJieba                m_jieba;        
    std::vector<WebPage>    m_pageLib;
    std::unordered_map<int, std::pair<int,int>>     m_offsetLib;
    std::unordered_map<std::string, std::vector<std::pair<int, double>>>  m_invertIndexTable;
};







}//end of namespace wd

