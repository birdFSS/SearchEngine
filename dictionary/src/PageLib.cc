#include "../include/PageLib.h"
#include "../include/RssReader.h"

using std::vector;
using std::string;
namespace wd
{

void PageLib::create()
{
    m_scan();       //调用travers遍历目录
    vector<string>& filePaths = m_scan.getFilePaths(); 
    for(auto iter = filePaths.begin(); iter != filePaths.end(); ++iter)
    {
        m_reader.parseRss(iter->c_str());
    }
}

void PageLib::store()
{
    m_reader.dump(m_conf.getConfigMap().at("pagelib"),m_conf.getConfigMap().at("offsetlib"));
    
}







}//wd

