#pragma once
#include <string>
#include <map>
#include <vector>
#include <set>
namespace wd
{
class Configuration;
class SplitTool;


class WebPage
{
    friend bool operator==(const WebPage& lhs, const WebPage& rhs);
    friend bool operator<(const WebPage& lhs, const WebPage& rhs);
public:
    WebPage(const std::string& doc, Configuration & config, SplitTool & splitTool);
    WebPage(std::string&& doc, Configuration & config, SplitTool & splitTool);
    ~WebPage() {}

    WebPage(const WebPage& rhs) = default;
    WebPage(WebPage&& rhs) noexcept; 

    WebPage& operator=(const WebPage& page);
    WebPage& operator=(WebPage&& page) noexcept;

    int getDocId();
    void setDocId(int i) { m_docId = i; }
    std::string getDoc();

    std::map<std::string, int> & getWordsMap();

    void show() const;



private:
    void processDoc(const std::string& doc, Configuration& config, SplitTool& splitTool);
    void calcTopK(std::vector<std::string> & wordsVec, int k, std::set<std::string> & stopWords);

private:
    const static int    TOPK_NUMBER = 10;
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

