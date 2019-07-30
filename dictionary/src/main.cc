#include "../include/Configuration.h"
#include "../include/DictProducer.h"
#include "../include/CppJieba.h"
#include "../include/IndexProducer.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::string;


const char * const ConfigFile = "../conf/config.utf8";

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
    auto confMap = conf.getConfigMap();

    wd::DictProducer dict(confMap["en_dir"]);
    wd::DictProducer CNdict(confMap["cn_dir"]);

    dict.showFilePath();
    CNdict.showFilePath();
}

void test2()
{
    //测试忽视的字词
    wd::Configuration conf(ConfigFile);
    auto confMap = conf.getConfigMap();

    wd::DictProducer dict(confMap["en_dir"]);
    wd::DictProducer CNdict(confMap["cn_dir"]);

    dict.setIgnoreWords(confMap["en_ignore"]);
    CNdict.setIgnoreWords(confMap["cn_ignore"]);
    
    //dict.showIgnoreWords();
    CNdict.showIgnoreWords();
}

void test3()
{
    //测试中文分词，构建中文词典
    std::shared_ptr<wd::SplitTool> pst(new wd::CppJieba());
    wd::Configuration conf(ConfigFile);
    auto confMap = conf.getConfigMap();

    wd::DictProducer CNdict(confMap["cn_dir"], pst);
    CNdict.setIgnoreWords(confMap["cn_ignore"]);
    CNdict.buildCNDict();
    CNdict.storeDict(confMap["cn_dict"]);
}

void test4()
{
    //构建英文词典
    wd::Configuration conf(ConfigFile);
    auto confMap = conf.getConfigMap();
    
    wd::DictProducer dict(confMap["en_dir"]);
    dict.setIgnoreWords(confMap["en_ignore"]);
    dict.buildDict();
    //dict.showDict();
    dict.storeDict(confMap["en_dict"]);

    //测试构建索引
    std::ofstream ofs("../data/index.utf8");
    wd::IndexProducer indexPro;
    indexPro.createIndex(dict, dict, ofs);

}

void test5()
{
    //测试中英文词典索引
    wd::Configuration conf(ConfigFile);
    auto confMap = conf.getConfigMap();
    
    wd::DictProducer dict(confMap["en_dir"]);
    dict.setIgnoreWords(confMap["en_ignore"]);
    dict.buildDict();

    std::shared_ptr<wd::SplitTool> pst(new wd::CppJieba());
    wd::DictProducer CNdict(confMap["cn_dir"], pst);
    CNdict.setIgnoreWords(confMap["cn_ignore"]);
    CNdict.buildCNDict();

    //测试构建索引
    std::ofstream ofs("../data/index.utf8");
    wd::IndexProducer indexPro;
    indexPro.createIndex(dict, CNdict, ofs);
    
}

int main()
{
    //test3();  //中文词典测试
    //test4();  //英文词典测试
    test5();
    return 0;
}



