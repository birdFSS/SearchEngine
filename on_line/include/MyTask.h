#pragma once
#include "TcpConnection.h"
#include <vector>
#include <string>
#include <set>
#include <queue>
#define WORD_NUMBER 5

#define MYTASK_DEBUG 1

namespace wd
{

class Cache;
struct MyResult
{
    MyResult(const std::string& word, int freq, int dist) :
        m_word(word),
        m_freq(freq),
        m_dist(dist)
    {}
    std::string m_word; //候选词
    int m_freq; //词频
    int m_dist; //最小距离
};

class MyCompare
{
public:
    //小顶堆，最上面实际优先级最低，最多保存10个
    bool operator()(const MyResult& lhs, const MyResult& rhs)
    {
        if(lhs.m_dist != rhs.m_dist)
        {
            return lhs.m_dist < rhs.m_dist;
        }else if(lhs.m_freq != rhs.m_freq)
        {
            return lhs.m_freq > rhs.m_freq;
        }else{
            return lhs.m_word < rhs.m_word;
        }
    }
};


class MyTask
{
public:
    MyTask(const std::string& word, int peerfd,const TcpConnectionPtr& conn):
        m_queryWord(word),
        m_peerfd(peerfd),
        m_conn(conn)
    {}
    ~MyTask() {}
    
    void excute(); //执行查询,供给线程调用
    void queryIndexTable(); //查询索引
    void statistic(std::set<int> & iset, int* array);   //进行计算
    int distance(const std::string& rhs);
    void response(Cache & cache);
    //void response();

    void showQueue();   //test

private:
    int editDistance(const std::string & lhs, const std::string &rhs);
    std::size_t length(const std::string &str);
    size_t nBytesCode(const char ch);
    bool isVisited(int *array, int line_no);
private:
    std::string m_queryWord;
    int m_peerfd;
    TcpConnectionPtr  m_conn;
    std::priority_queue<MyResult, std::vector<MyResult>, MyCompare> m_resultQue;
};







}//end of namespace wd

