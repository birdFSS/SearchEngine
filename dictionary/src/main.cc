#include "../include/Configuration.h"
#include "../include/DictProducer.h"
#include "../include/CppJieba.h"
#include "../include/IndexProducer.h"
#include "../include/RssReader.h"
#include "../include/Mylog.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::string;
using namespace tinyxml2;


const char * const ConfigFile = "../conf/myconf.conf";

void test0()
{
    //测试读取配置文件信息
    wd::Configuration conf(ConfigFile);
    auto confMap = conf.getConfigMap();
    for(auto i : confMap)
    {
        cout << i.first << "--->" << i.second << endl;
    }
}

void test1()
{
    //测试读取语料库文件的绝对位置
    wd::Configuration conf(ConfigFile);
    auto& confMap = conf.getConfigMap();

    wd::DictProducer sourcePath(confMap["sourcePath"]);

    sourcePath.showFilePath();
}

void test2()
{
    tinyxml2::XMLDocument doc;
    if(doc.LoadFile("../source/coolshell.xml"))
    {
        logError("error load");
        return ;
    }
    RssReader myReader;
    RssItem tmpItem;
    myReader.parseRss("../source/coolshell.xml");
    myReader.dump("../data/test.lib");
}

int main()
{
    //test3();  //中文词典测试
    //test4();  //英文词典测试
    test2();
    return 0;
}



