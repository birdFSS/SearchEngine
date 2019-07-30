#include "../include/WebPage.h"

using std::string;
using std::vector;
using std::map;
using std::set;

namespace wd
{


bool operator==(const WebPage& lhs, const WebPage& rhs);
bool operator<(const WebPage& lhs, const WebPage& rhs);

WebPage::WebPage(std::string& doc, Configuration & config, CppJieba &  jieba)
{

}

int WebPage::getDocld()
{
    return m_docId;
}

std::string WebPage::getDoc()
{
    return m_doc;
}

std::map<std::string, int> & WebPage::getWordsMap()
{
    return m_wordsMap;
}

void WebPage::processDoc(const std::string& doc, Configuration& config, CppJieba& jieba)
{

}

void WebPage::calcTopK(std::vector<std::string> & wordsVec, int k, std::set<std::string> & stopWordList)
{

}












}//wd
