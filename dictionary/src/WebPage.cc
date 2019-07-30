#include "../include/WebPage.h"
#include "../include/SplitTool.h"
#include "../include/Configuration.h"
#include "../include/tinyxml2.h"
#include "../include/Mylog.h"
#include <algorithm>

using std::string;
using std::vector;
using std::map;
using std::set;

namespace wd
{


bool operator==(const WebPage& lhs, const WebPage& rhs)
{

}

bool operator<(const WebPage& lhs, const WebPage& rhs)
{

}

WebPage::WebPage(std::string& doc, Configuration & config, SplitTool &  splitTool) :
    m_doc(doc)
{
    processDoc(doc, config, splitTool);
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

void WebPage::processDoc(const std::string& doc, Configuration& config, SplitTool& splitTool)
{
    auto wordsVec = splitTool.cut(doc);
    calcTopK(wordsVec, TOPK_NUMBER, config.getStopWordList());

    string::size_type pos, pos_next ;
    if((pos = doc.find("<docid>", 0)) != string::npos)
    {
        m_docId = stoi(doc.substr(pos + strlen("<docid>")));
    }else{
        logError("not find doc id");
        return ;
    }

    if((pos = doc.find("<url>", pos)) != string::npos)
    {
        if((pos_next = doc.find("</url>", pos)) != string::npos)
        {
            m_docUrl.assign(doc,pos + strlen("<url>"), pos_next - pos - strlen("<url>")); 
        }else{
            logError("not find /url");
            return;
        }
    }else{
        logError("not find url");
        return;
    }

    if((pos = doc.find("<title>", pos)) != string::npos)
    {
        if((pos_next = doc.find("</title>", pos)) != string::npos)
        {
            m_docTitle.assign(doc,pos + strlen("<title>"), pos_next - pos - strlen("<title>")); 
        }else{
            logError("not find /title");
            return ;
        }
    }else{
        logError("not find title");
        return ;
    }


    if((pos = doc.find("<content>", pos)) != string::npos)
    {
        if((pos_next = doc.find("</content>", pos)) != string::npos)
        {
            m_docContent.assign(doc,pos + strlen("<content>"), pos_next - pos - strlen("<content>")); 
        }else{
            logError("not find /content");
            return;
        }
    }else{
        logError("not find content");
        return;
    }
}

void WebPage::calcTopK(std::vector<std::string> & wordsVec, int k, std::set<std::string> & stopWordList)
{
    for(auto iter = wordsVec.begin(); iter != wordsVec.end(); ++iter)
    {
        if(stopWordList.find(*iter) == stopWordList.end())
        {
            ++m_wordsMap[*iter];
        }
    }
    vector<string> tmpVec(m_wordsMap.begin(), m_wordsMap.end());
    sort(tmpVec.begin(),tmpVec.end(), 
        [](const std::pair<string, int>& lhs, const std::pair<string, int> & rhs) { return lhs.second > rhs.second;}
        );
    m_topWords.assign(tmpVec.begin(), tmpVec.begin() + k);
}












}//wd
