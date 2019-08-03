#pragma once
#include "Configuration.h"
#include "CppJieba.h"
#include "WebPage.h"

#include <unordered_map>
#include <vector>
#include <set>
#include <string>

namespace wd
{

class WordQuery
{
public:
    WordQuery(Configuration & conf);
    ~WordQuery() = default; 

    std::string doQuery(const std::string& str);

private:
    void loadLibarary();
    std::vector<double> getQueryWordsWeightVector(std::vector<std::string> & queryWord);
    bool excuteQuery(const std::vector<std::string> & queryWords, 
                     std::vector<std::pair<int, std::vector<double>>> & resultVec);
    std::string createJson(std::vector<int> & docIdVec, const std::vector<std::string> & queryWords);
    std::string returnNoAnswer();
private:
    Configuration & m_conf;
    CppJieba        m_jieba;
    std::unordered_map<int, WebPage>         m_pageLib;
    std::unordered_map<int, std::pair<int, int>>      m_offsetLib;
    std::unordered_map<std::string, std::set<std::pair<int, double>>>   m_invertIndexTable;
};







}//end of namespace wd

