#include "../include/Configuration.h"
#include "../include/CppJieba.h"
#include "../include/DirScanner.h"
#include "../include/RssReader.h"
#include "../include/Mylog.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <fstream>

using std::ifstream;

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::string;
using namespace tinyxml2;

const char * const ConfigFile = "../conf/myconf.conf";

//测试读取配置文件信息
void test0();

//测试读取语料库文件的绝对位置
void test1();
//测试读取解析xml文件的绝对路径
void test2();
//测试offset文件是否正确
void test3();

int main()
{
    //test0();
    //test1();
    test2();
    return 0;
}

//测试读取配置文件信息
void test0()
{
    wd::Configuration conf(ConfigFile);
    auto confMap = conf.getConfigMap();
    for(auto i : confMap)
    {
        cout << i.first << "--->" << i.second << endl;
    }
}

//测试读取语料库文件的绝对位置
void test1()
{
    wd::Configuration conf(ConfigFile);
    
    wd::DirScanner dirScanner(conf);
    dirScanner();
    dirScanner.show();
}

void test2()
{
    RssReader myReader;
    myReader.parseRss("../source/s1/techweb.xml");
    //myReader.parseRss("../source/s1/coolshell.xml");
    myReader.dump("../data/test1.lib", "../data/offset_test1.lib");
}

//测试offset文件是否正确
void test3()
{
    ifstream ifs_doc("../data/test.lib");
    ifstream ifs_offset("../data/offset_test.lib");

    size_t id;
    size_t offset, len;
    int i = 0;
    while(i++ < 3)      //这个数字为几，代表第几篇文章
    {
        ifs_offset >> id >> offset >> len;
    }
    ifs_doc.seekg(offset, std::fstream::beg);
    char* buff = new char[len+1]();
    ifs_doc.read(buff, len);
    printf("%s$$", buff); //里面包括了最后的换行符，正确,用$$的位置判断
}