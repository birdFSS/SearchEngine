#include "../include/WordQuery.h"
#include "../include/Mylog.h"
#include <json/json.h>
#include <set>
using std::set;
using std::map;
using std::make_pair;
using std::string;
using std::ifstream;
using std::vector;
using std::pair;

namespace wd
{

WordQuery::WordQuery(Configuration& conf) :
    m_conf(conf)
{
    loadLibarary();
}

std::string WordQuery::doQuery(const std::string& str)
{
    vector<string> queryWords;
    if(str.size() > 0)
    {
        queryWords = m_jieba.cut(str);
    }
    
    auto& stopWords = m_conf.getStopWords();
    for(auto& item : queryWords)
    {
        logInfo("%s", item.c_str());
        if(stopWords.find(item) != stopWords.end())
        {
            continue;
        }

        auto uit = m_invertIndexTable.find(item);
        if(uit == m_invertIndexTable.end())
        {
            logInfo("can not find %s", item.c_str());
            return returnNoAnswer();
        }
    }

    vector<double> weightVec = getQueryWordsWeightVector(queryWords);
    SimilarityCompare similarityCmp(weightVec);

    vector<pair<int, vector<double>>> resultVec;
    if(excuteQuery(queryWords, resultVec))
    {
        stable_sort(resultVec.begin(), resultVec.end(), similarityCmp);
        vector<int> docIdVec;

        for(auto item: resultVec)
        {
            docIdVec.push_back(item.first);
        }

        return createJson(docIdVec, queryWords);
    }else{
        return returnNoAnswer();
    }
}

void WordQuery::loadLibarary()
{
    ifstream ifs_pageLib(m_conf.getConfigMap().at("pageLib"));
    ifstream ifs_offsetLib(m_conf.getConfigMap().at("offsetLib"));

    if((!ifs_pageLib.good()) || (!ifs_pageLib.good()))
    {
        logError("open page lib or offset lib error");
    }
    int docId = 0;
    size_t offset, len;
    while(ifs_offsetLib >> docId >> offset >> len)
    {
        string doc;
        doc.resize(len , ' ');

        ifs_pageLib.seekg(offset, ifs_pageLib.beg);
        ifs_pageLib.read(&*doc.begin(), len);

        WebPage webPage(doc , m_conf, m_jieba);
        m_pageLib.insert(make_pair(docId, webPage));

        m_offsetLib.insert(make_pair(docId, make_pair(offset, len)));

    }
    ifs_offsetLib.close();
    ifs_pageLib.close();

    //load invertIndexTable
    ifstream ifs_invertIndex(m_conf.getConfigMap().at("invertIndexTable"));
    if(!ifs_invertIndex.good())
    {
        logError("open invertIndexTable error");
    }
    string word, line;
    double weight;
    while(getline(ifs_invertIndex, line))
    {
        std::istringstream iss(line);
        iss >> word;
        set<pair<int, double>> setID;
        while(iss >> docId >> weight)
        {
            setID.insert(make_pair(docId, weight));
        }
        m_invertIndexTable.insert(make_pair(word, setID));
    }
    ifs_invertIndex.close();
}

vector<double> WordQuery::getQueryWordsWeightVector(vector<string> & queryWords)
{
    map<string, int> queryWordMap;
    for(size_t i=0;i < queryWords.size(); ++i)
    {
        ++queryWordMap[queryWords[i]];
    }
    
    int totalPageNum = m_offsetLib.size();
    double weight = 0, idf = 0, weightSum = 0;
    vector<double> weightVector;
    int df=0, tf = 0;

    for(auto& item : queryWords)
    {
        df = m_invertIndexTable[item].size();
        idf = log(static_cast<double>(totalPageNum) / (df + 1)) / log(2);
        tf = queryWordMap[item];
        weight = idf * tf;
        weightSum += pow(weight, 2);
        weightVector.push_back(weight);
    }

    for(auto & item : weightVector)
    {
        item /= sqrt(weightSum);
    }
    return weightVector;
}

bool WordQuery::excuteQuery(const std::vector<std::string> & queryWords,
                 std::vector<std::pair<int, std::vector<double>>> & resultVec)
{
    if(queryWords.size() == 0)
    {
        logError("empty string not find");
        return false;
    }

    typedef set<pair<int, double>>::iterator setIter;
    vector<pair<setIter, int>> iterVec;
    int minIterNum = 0x7FFFFFFF;
    for(auto item : queryWords)
    {
        int sz = m_invertIndexTable[item].size();
        if(0 == sz)
        {
            return false;
        }

        if(minIterNum > sz)
        {
            minIterNum = sz;
        }

        iterVec.push_back(make_pair(m_invertIndexTable[item].begin(), 0));
    }

    bool isExiting = false;

    while(!isExiting)
    {
        size_t idx = 0;
        for(;idx != iterVec.size() - 1; ++idx)
        {
            if((iterVec[idx].first)->first != iterVec[idx + 1].first->first)
                break;
        }

        if(idx == iterVec.size() - 1)
        {
            vector<double> weightVec;
            int docId = iterVec[0].first->first;
            for(idx = 0; idx != iterVec.size(); ++ idx)
            {
                weightVec.push_back(iterVec[idx].first->second);
                ++(iterVec[idx].first);
                ++(iterVec[idx].second);
                if(iterVec[idx].second == minIterNum)
                {
                    isExiting = true;
                }
            }
            resultVec.push_back(make_pair(docId, weightVec));
        }else{
            int minDocId = 0x7FFFFFFF;
            int iterIdx;
            for(idx = 0; idx != iterVec.size(); ++idx)
            {
                if(iterVec[idx].first->first < minDocId)
                {
                    minDocId = iterVec[idx].first->first;
                    iterIdx = idx;
                }
            }

            ++(iterVec[iterIdx].first);
            ++(iterVec[iterIdx].second);
            if(iterVec[iterIdx].second == minIterNum)
            {
                isExiting = true;
            }
        }
    }
    return true;
}

std::string WordQuery::createJson(std::vector<int> & docIdVec, const std::vector<std::string> & queryWords)
{
    Json::Value root;
    Json::Value arr;
    int cnt = 0;
    for(auto id : docIdVec)
    {
        string summary = m_pageLib[id].summary(queryWords);
        string title = m_pageLib[id].getTitle();
        string url = m_pageLib[id].getUrl();

        Json::Value elem;
        elem["title"] = title;
        elem["summary"] = summary;
        elem["url"] = url;
        arr.append(elem);
        if(++cnt == 100)
        {
            break;
        }
    }

    root["files"] = arr;
    Json::StyledWriter writer;
    return writer.write(root);
    return string(" ");
}

std::string WordQuery::returnNoAnswer()
{
    Json::Value root;
    Json::Value arr;
    Json::Value elem;

    elem["title"] = "404, not found";
    elem["summary"] = "亲, I can't find what you want. What a pity";
    elem["url"] = "";
    arr.append(elem);
    root["files"] = arr;
    Json::StyledWriter writer;
    return writer.write(root);
}





















}
