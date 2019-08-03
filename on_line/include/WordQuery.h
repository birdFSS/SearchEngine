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

struct SimilarityCompare
{
    SimilarityCompare(std::vector<double> & base) :
        m_base(base)
    {}

    bool operator()(const std::pair<int, std::vector<double>> & lhs, 
                  const std::pair<int, std::vector<double>> &rhs)
    {
        double lhsCrossProduct = 0;
        double rhsCrossProduct = 0;
        double lhsVectorLength = 0;
        double rhsVectorLength = 0;

        for(size_t index = 0; index != m_base.size(); ++index)
        {
            lhsCrossProduct += (lhs.second)[index] * m_base[index];
            rhsCrossProduct += (rhs.second)[index] * m_base[index];
            lhsVectorLength += pow((lhs.second)[index], 2);
            rhsVectorLength += pow((rhs.second)[index], 2);
        }

        if(lhsCrossProduct / sqrt(lhsVectorLength) < rhsCrossProduct / sqrt(rhsVectorLength))
        {
            return false;
        }else{
            return true;
        }
    }
    std::vector<double> m_base;
        
};

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

