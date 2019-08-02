#include "../include/PageLib.h"
#include "../include/RssReader.h"
#include "../include/Mylog.h"

using std::vector;
using std::string;
namespace wd
{

void PageLib::create()
{
    m_scan();       //调用travers遍历目录
    //m_scan.show();
    vector<string>& filePaths = m_scan.getFilePaths(); 
    for(auto iter = filePaths.begin(); iter != filePaths.end(); ++iter)
    {
        //logInfo("%s", iter->c_str());
        m_reader.parseRss(iter->c_str());
    }
}

void PageLib::store()
{
    m_reader.dump(m_conf.getConfigMap().at("pageLib"),m_conf.getConfigMap().at("offsetLib"));
}







}//wd

