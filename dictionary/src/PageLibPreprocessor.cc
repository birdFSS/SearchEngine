#include "../include/PageLibPreprocessor.h"
#include "../include/Configuration.h"
#include "../include/Mylog.h"
#include <set>
#include <fstream>
using std::ifstream;
using std::set;

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
    int docId;
    size_t offset, len;
    while(ifs_index >> docId >> offset >> len)
    {
        ifs_doc.seekg(offset, ifstream::beg);
        char* buff = new char[len + 1]();
        ifs_doc.read(buff, len);
        m_pageLib.push_back(WebPage(std::string(buff), m_conf, m_jieba));
        delete [] buff;
    }
}

void PageLibPreprocessor::cutRedundantPages()
{
    auto back_iter = m_pageLib.end();
    for(auto i= m_pageLib.begin();i != back_iter;++i)
    {
        for(auto j=i+1;j != back_iter;)
        {
            if(*i == *j)
            {
                --back_iter;
                if(j != back_iter)
                {
                    auto temp = *j;
                    *j = *back_iter;
                    *back_iter = temp;
                }
            }else{
                ++j;
            }
        }
    }

    m_pageLib.erase(back_iter, m_pageLib.end());
}

void PageLibPreprocessor::buildInvertIndexTable()
{

}

void PageLibPreprocessor::storeOnDisk()
{

}




}//end wd
