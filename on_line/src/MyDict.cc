#include "../include/MyDict.h"
#include "../include/Mylog.h"
#include <fstream>
#include <sstream>
#include <iostream>

using std::istringstream;
using std::ofstream;
using std::ifstream;
using std::cout;
using std::endl;
using std::string;
using std::make_pair;


namespace wd
{

MyDict * MyDict::m_self = nullptr;    //因为需要由客户端初始化，所以要饿汉模式

void MyDict::init(const std::string& dictEnPath, const std::string& dictCnPath)
{
    ifstream ifs(dictEnPath);
    string word;
    int freq;
    while(ifs >> word >> freq)
    {
        m_dict.push_back(make_pair(word, freq));
    }
    ifs.clear();
    ifs.close();

    ifs.open(dictCnPath);
    while(ifs >> word >> freq)
    {
        m_dict.push_back(make_pair(word, freq));
    }
    

}

void MyDict::setIndexTable(const std::string& IndexTablePath)
{
    ifstream ifs(IndexTablePath);
    string line;
    while(getline(ifs, line))
    {
        string ch;
        int line_no;
        istringstream iss(line);
        iss >> ch;      //先得到索引字符
        while(iss >> line_no)
        {
            m_index_table[ch].insert(line_no);
        }
    }
}


void MyDict::init(const std::string& dictEnPath, const std::string& dictCnPath,const std::string& IndexTablePath)
{
    init(dictEnPath, dictCnPath);
    setIndexTable(IndexTablePath);
}

void MyDict::showTableInTestFile(const std::string& path)
{
    //test
    ofstream ofs(path);
    for(auto& i : m_index_table)
    {
        ofs << i.first << " ";
        for(auto& num : i.second)
        {
            ofs << num << " ";
        }
        ofs << endl;
    }
    logInfo("索引表输出完毕，输出文件名为:%s\n", path);
}

void MyDict::showDictInTestFile(const std::string& path)
{
    //test
    ofstream ofs(path);
    for(auto& i : m_dict)
    {
        ofs << i.first << " " << i.second << endl;
    }
    logInfo("词典输出完毕，输出文件名为:%s\n", path);
}











}//wd
