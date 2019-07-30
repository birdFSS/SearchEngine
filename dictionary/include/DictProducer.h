#pragma once
#include <string>
#include <map>
#include <set>
#include <vector>
#include <memory>
using std::string;
using std::map;
using std::set;
using std::vector;

namespace wd
{

class SplitTool;

class DictProducer
{
    friend class IndexProducer;
public:
    DictProducer(const string& dir, std::shared_ptr<SplitTool> splitTool); //中文
    DictProducer(const string& dir);        //英文
    DictProducer(const string& dir, const string &ignorePath);    //英文
    
    void setIgnoreWords(const string & path) ;
    void buildDict();
    void buildCNDict();

    size_t nBytesCode(const char ch);       //判断是否是中文字符
    void storeDict(const string& filePath);

    void showFilePath() const; //测试接口
    void showDict() const; //测试接口
    void showIgnoreWords() const;
    void getFileAbosolutePath() ;  //获取文件的绝对路径
    void pushDict(const string& word);  //存储某个单词
    

private:
    void DealOtherCharForCN(string & str);
    void getDirAbosolutePath();
    void DealOtherChar(string& str);    //处理标点和将大写字母变为小写
private:
    string m_dir;       //语料库文件存放路径
    vector<string> m_filePath;  //语料库文件的绝对路劲集合
    map<string, int> m_dict;    //单词 词频
    set<string> m_ignore;       //忽略一些简单的单词
    std::shared_ptr<SplitTool> m_splitTool;
};







}//end of namespace wd

