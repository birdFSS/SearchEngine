#pragma once
#include <string>
#include <map>
#include <vector>
#include <set>
namespace wd
{
class Configuration;
class CppJieba;


class WebPage
{
    friend bool operator==(const WebPage& lhs, const WebPage& rhs);
    friend bool operator<(const WebPage& lhs, const WebPage& rhs);
public:
    WebPage(std::string& doc, Configuration & config, CppJieba &  jieba);
    ~WebPage() {}

    int getDocld();
    std::string getDoc();

    std::map<std::string, int> & getWordsMap();

private:
    void processDoc(const std::string& doc, Configuration& config, CppJieba& jieba);
    void calcTopK(std::vector<std::string> & wordsVec, int k, std::set<std::string> & stopWordList);

private:
    const static int    TOPK_NUMBER = 20;
    std::string         m_doc;
    int                 m_docId;
    std::string         m_docTitle;
    std::string         m_docUrl;
    std::string         m_docContent;
    std::string         m_docSummary;
    std::vector<std::string>       m_topWords;
    std::map<std::string, int>     m_wordsMap;

};







}//end of namespace wd
