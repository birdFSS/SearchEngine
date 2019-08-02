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
    for(auto iter_lhs = lhs.m_topWords.begin(); iter_lhs != lhs.m_topWords.end(); ++iter_lhs)
    {
        for(auto iter_rhs = rhs.m_topWords.begin(); iter_rhs != rhs.m_topWords.end(); ++iter_rhs)
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

WebPage::WebPage(const std::string& doc, Configuration & config, SplitTool &  splitTool) :
    m_doc(doc)
{
    processDoc(m_doc, config, splitTool);
}

WebPage::WebPage(std::string&& doc, Configuration & config, SplitTool &  splitTool) :
    m_doc(doc)
{
    processDoc(m_doc, config, splitTool);
}

#if 0       //使用=default代替
WebPage::WebPage(const WebPage& rhs) :
    m_doc(rhs.m_doc),
    m_docId(rhs.m_docId),
    m_docTitle(rhs.m_docTitle),
    m_docUrl(rhs.m_docUrl),
    m_docContent(rhs.m_docContent),
    m_docSummary(rhs.m_docSummary),
    m_topWords(rhs.m_topWords.begin(), rhs.m_topWords.end()),
    m_wordsMap(rhs.m_wordsMap.begin(), rhs.m_wordsMap.end())
{

}

#endif
WebPage::WebPage(WebPage&& rhs) noexcept
{
    //logInfo("WebPage::WebPage(WebPage&& rhs) noexcept");
    m_doc.swap(rhs.m_doc);
    m_docId = rhs.m_docId;
    m_docTitle.swap(rhs.m_docTitle);
    m_docUrl.swap(rhs.m_docUrl);
    m_docContent.swap(rhs.m_docContent);
    m_docSummary.swap(rhs.m_docSummary);
    m_topWords.swap(rhs.m_topWords);
    m_wordsMap.swap(rhs.m_wordsMap);
}

WebPage& WebPage::operator=(const WebPage& page) 
{
    //logInfo("WebPage& WebPage::operator=(const WebPage& page)");
    if(this != &page)
    {
        m_doc = page.m_doc;
        m_docId = page.m_docId;
        m_docTitle = page.m_docTitle;
        m_docUrl = page.m_docUrl;
        m_docContent = page.m_docContent;
        m_docSummary = page.m_docSummary;
        m_topWords.assign(m_topWords.begin(), m_topWords.end());
        m_wordsMap.clear();
        for(auto& i : page.m_wordsMap)
        {
            m_wordsMap.insert(i);
        }
    }
    return *this;
}

WebPage& WebPage::operator=(WebPage&& page) noexcept
{
    //logInfo(" WebPage::WebPage(WebPage&& rhs) noexcept");
    if(this != &page)
    {
        m_doc.swap(page.m_doc);
        m_docId = page.m_docId;
        m_docTitle.swap(page.m_docTitle);
        m_docUrl.swap(page.m_docUrl);
        m_docContent.swap(page.m_docContent);
        m_docSummary.swap(page.m_docSummary);
        m_topWords.swap(page.m_topWords);
        m_wordsMap.swap(page.m_wordsMap);
    }

    return *this;
}

int WebPage::getDocId()
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
    calcTopK(wordsVec, TOPK_NUMBER, config.getStopWords());

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


void WebPage::calcTopK(std::vector<std::string> & wordsVec, int k, std::set<std::string> & stopWords)
{
    for(auto iter = wordsVec.begin(); iter != wordsVec.end(); ++iter)
    {
        if(stopWords.find(*iter) == stopWords.end())
        {
            if(!isspace((*iter)[0]))
            {
                ++m_wordsMap[*iter];
            }else{
                logError("--");
            }
        }
    }

    if(m_wordsMap.find(" ") != m_wordsMap.end())
    {
        logError("$%s$", m_wordsMap.find(" ")->first.c_str());
    }
    vector<std::pair<string, int>> tmpVec(m_wordsMap.begin(), m_wordsMap.end());

    sort(tmpVec.begin(),tmpVec.end(), 
         [](const std::pair<string, int>& lhs, const std::pair<string, int> & rhs) { return lhs.second > rhs.second;}
        );

    
    for(auto& pair_si : tmpVec)
    {

        m_topWords.push_back(pair_si.first);

        logInfo("%s-->%ld", pair_si.first.c_str(), pair_si.second);

        if(--k == 0)
        {
            break;
        }
    }
}


void WebPage::show() const
{
    //logInfo("%s", m_doc.c_str());
    logInfo("id = %d", m_docId);
    //logInfo("title =$%s$", m_docTitle.c_str());
    //logInfo("url=$%s$", m_docUrl.c_str());
    //logInfo("content=$%s$", m_docContent.c_str());
    for(auto& word : m_topWords)
    {
        logInfo("%s", word.c_str());
    }
}









}//wd
