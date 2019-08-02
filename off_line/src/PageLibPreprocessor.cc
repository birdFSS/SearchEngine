#include "../include/PageLibPreprocessor.h"
#include "../include/Configuration.h"
#include "../include/Mylog.h"
#include <set>
#include <map>
#include <fstream>
using std::ifstream;
using std::map;
using std::set;
using std::string;

#define PAGELIBPRO_DEBUG 1

namespace wd
{

PageLibPreprocessor::PageLibPreprocessor(Configuration& conf) :
    m_conf(conf)
{

}

void PageLibPreprocessor::doProcess()
{
    readInfoFromFile();
    cutRedundantPages();
#if PAGELIBPRO_DEBUG
    size_t i = 0;
    while(++i != m_pageLib.size())
    {
        logInfo("-----------------%ld-------------", i);
        m_pageLib[i].show();
    }

#endif
    buildInvertIndexTable();
    storeOnDisk();
}


void PageLibPreprocessor::readInfoFromFile()
{
    ifstream ifs_doc(m_conf.getConfigMap().at("pageLib"));
    ifstream ifs_index(m_conf.getConfigMap().at("offsetLib"));
    if((!ifs_doc.good()) || (!ifs_index.good()))
    {
        logError("page or offset lib open error!");
        exit(-1);
    }

    int docId;
    size_t offset, len;
    while(ifs_index >> docId >> offset >> len)
    {
        ifs_doc.seekg(offset, ifstream::beg);
        string doc;
        doc.resize(len, ' ');
        ifs_doc.read(&*doc.begin(), len);
        m_pageLib.push_back(WebPage(std::move(doc), m_conf, m_jieba));

        m_offsetLib.insert(std::make_pair(docId, std::make_pair(offset, len)));
    }
}

void PageLibPreprocessor::cutRedundantPages()
{
    auto back_iter = m_pageLib.end();
    for(auto i= m_pageLib.begin();i != back_iter;++i)
    {
        for(auto j=i+1;j != back_iter; ++j)
        {
            if(*i == *j)
            {
                --back_iter;
                if(j != back_iter)
                {
                    //不需要交换，因为*j是不需要的
                    //auto temp = *j;
                    *j = *back_iter;
                    //*back_iter = temp;
                    --j;
                }
            }
        }
    }

    m_pageLib.erase(back_iter, m_pageLib.end());
}

void PageLibPreprocessor::buildInvertIndexTable()
{
    //先建立词语与文档id的联系，保存词语在对应文档的次数
    for(auto& page : m_pageLib)
    {
        for(auto& wordFreq : page.getWordsMap())
        {
            m_invertIndexTable[wordFreq.first].push_back(std::make_pair(
                page.getDocId(), wordFreq.second ));
        }
    }

    //建立倒排索引，权重计算 TF-IDF算法
    map<int, double> weightSum; //保存每一篇文档中所有词的权重平方和

    size_t totalPageNum = m_pageLib.size();
    for(auto &item : m_invertIndexTable)
    {
        int df = item.second.size();
        double idf = log(static_cast<double>(totalPageNum)/(df+1)) / log(2);

        for(auto& sItem : item.second)
        {
            double weight = sItem.second * idf; //词频tf * idf
            weightSum[sItem.first] += pow(weight, 2);
            sItem.second = weight;
        }
    }

    for(auto& item : m_invertIndexTable)
    {
        //归一化处理
        for(auto & sItem : item.second)
        {
            sItem.second = sItem.second / sqrt(weightSum[sItem.first]);
        }
    }

#if PAGELIBPRO_DEBUG
    for(auto& item : m_invertIndexTable)
    {
        logDebug("%s", item.first.c_str());
        for(auto& sItem : item.second)
        {
            logDebug("%d-->%lf", sItem.first, sItem.second);
        }
    }
#endif
}

void PageLibPreprocessor::storeOnDisk()
{

}




}//end wd
