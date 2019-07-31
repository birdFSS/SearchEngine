#include "../include/WebPage.h"
#include "../include/SplitTool.h"
#include "../include/Configuration.h"
#include "../include/Mylog.h"
#include <string.h>
#include <algorithm>

using std::string;
using std::vector;
using std::map;
using std::set;

namespace wd
{


bool operator==(const WebPage& lhs, const WebPage& rhs)
{
    int cnt = 0;
    for(auto iter_lhs = lhs.m_topWords.begin(); iter_lhs != lhs.m_topWords.begin() + 10; ++iter_lhs)
    {
        for(auto iter_rhs = rhs.m_topWords.begin(); iter_rhs != rhs.m_topWords.begin() + 10; ++iter_rhs)
        {
            if(*iter_lhs == *iter_rhs)
            {
                ++cnt;
                break;
            }
        }
    }
    return cnt > 6 ? true : false;
}

bool operator<(const WebPage& lhs, const WebPage& rhs)
{
    return lhs.m_docId < rhs.m_docId;
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
        m_docId = stoi(doc.substr(pos + 7));
    }else{
        logError("not find doc id");
        return ;
    }

    if((pos = doc.find("<url>", pos)) != string::npos)
    {
        if((pos_next = doc.find("</url>", pos)) != string::npos)
        {
            m_docUrl.assign(doc,pos + 5, pos_next - pos - 5);//strlen(<"url">)
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
            m_docTitle.assign(doc,pos + 7, pos_next - pos - 7);
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
        if((pos_next = doc.find_last_of("</content>", string::npos)) != string::npos)   //返回最后一位的下标，>的下标
        {
            m_docContent.assign(doc,pos + 9, pos_next - pos - 25);    // pos_next - (pos + 9) - 8
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
    vector<std::pair<string, int>> tmpVec(m_wordsMap.begin(), m_wordsMap.end());
    sort(tmpVec.begin(),tmpVec.end(), 
        [](const std::pair<string, int>& lhs, const std::pair<string, int> & rhs) { return lhs.second > rhs.second;}
        );

    for(auto& pair_si : tmpVec)
    {
        m_topWords.push_back(pair_si.first);
        if(--k == 0)
        {
            break;
        }
    }
}


void WebPage::show() const
{
    logInfo("%s", m_doc.c_str());
    logInfo("id = %d", m_docId);
    logInfo("title =$%s$", m_docTitle.c_str());
    logInfo("url=$%s$", m_docUrl.c_str());
    logInfo("content=$%s$", m_docContent.c_str());
    for(auto& word : m_topWords)
    {
        logInfo("%s", word.c_str());
    }
}









}//wd
