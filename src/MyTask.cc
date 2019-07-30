#include "../include/MyTask.h"
#include "../include/MyDict.h"
#include "../include/Thread.h"
#include "../include/CacheManager.h"
#include <json/json.h>
#include <algorithm>
#include <iostream>
using std::cout;
using std::endl;
using std::string;

namespace wd
{
inline
int triple_min(const int &a, const int &b, const int &c)
{
	return a < b ? (a < c ? a : c) : (b < c ? b : c);
}

void MyTask::showQueue()
{
    cout << " MyTask::showQueue()" << endl;
    cout << m_queryWord << endl;
    int i = 0;
    while(i++ < 10 && m_resultQue.size() > 0)
    {
        MyResult elem = m_resultQue.top();
        m_resultQue.pop();
        cout << "word = " << elem.m_word << "  frequency = " << elem.m_freq << "  distance =  " << elem.m_dist << endl;
    }

}

void MyTask::excute()
{   
    auto& cache = CacheManager::getInstance()->getCache(current_thread::CacheIndex);
    auto iter = cache.m_hashMap.find(m_queryWord);
    //cout << "map size = " << cache.m_hashMap.size() << endl;
    if((iter  != cache.m_hashMap.end()))
    {

#if MYTASK_DEBUG
        cout << "get Cache" << endl;
#endif

        m_conn->sendInLoop(iter->second->m_value); 
        cache.addElement(iter->first, iter->second->m_value);     //更新lru算法中位置
    }else{

#if MYTASK_DEBUG
        cout << "queryIndexTable ing..." << endl;
#endif

        queryIndexTable();
        response(cache);
    }
}

void MyTask::queryIndexTable()
{
    MyDict* pDict = MyDict::getInstance();
    auto& m_indexTable = pDict->getIndexTable();    

    int len = length(m_queryWord);
    int pos = 0;
    int step;  
    auto iter = m_indexTable.begin();

    int array[10000] = {0};
    //printf("word=%s$,size=%ld$\n", m_queryWord.c_str(), m_queryWord.size());
    for(int i=0; i< len; i++)
    {
        //printf("pos=%d$\n", pos);
        step = nBytesCode(m_queryWord[pos]);    //这个放在里面
        string oneWord(m_queryWord, pos, step); 
        //cout << oneWord << endl;
        if((iter = m_indexTable.find(oneWord)) != m_indexTable.end())
        {
            statistic(iter->second, array);
        }
        pos += step;
    }
}

bool MyTask::isVisited(int *array, int line_no)
{
    //位图
    int index = line_no / 32;
    int bit32 = line_no % 32;
    if((array[index] & (1 << bit32)) != 0)
    {
        return true;    //已经访问过了
    }else
    {
        array[index] = array[index] | (1 << bit32);
        return false;
    }
}

void MyTask::statistic(std::set<int> & iset, int* array)
{
    auto iter = iset.begin();
    int dist;

    MyDict* pDict = MyDict::getInstance();
    const auto & m_dict = pDict->getDict();

    while(iter != iset.end())
    {
        dist = distance(m_dict.at(*iter).first);
        MyResult result(m_dict.at(*iter).first, m_dict.at(*iter).second, dist);
        //去重，或者判断下重复的不加入
        if(!isVisited(array,*iter))
        {
            m_resultQue.push(result);
        }
        //当队列中元素超过10个时
        if(m_resultQue.size() > WORD_NUMBER)
        {
            m_resultQue.pop();  //将目前优先级队列中优先级最低的踢出去
        }
        //printf("4 *iter = %d\n", *iter);
        ++iter;
    }
}

int MyTask::distance(const std::string & rhs)
{
    return editDistance(m_queryWord, rhs);
}

void MyTask::response(Cache& cache)
{
    string jsonHead = "{\"" + m_queryWord + "\":[";
    string msg;
    int i = WORD_NUMBER;
    while(i-- != 0 && m_resultQue.size() != 0)
    {
        string tmp = m_resultQue.top().m_word;
        m_resultQue.pop();
        if(i != WORD_NUMBER - 1)
        {
            msg =  "\"" + tmp + "\"," + msg; 
        }else{
            msg = "\"" + tmp + "\"" + msg;
        }
    }
    msg = jsonHead + msg + "]}";

#if MYTASK_DEBUG
    printf("MyTask::response send msg = %s$$\n", msg.c_str());
#endif

    m_conn->sendInLoop(msg);
    cache.addElement(m_queryWord, msg);
}


/// 获取一个字节高位开头为1的个数
size_t MyTask::nBytesCode(const char ch)
{
	if(ch & (1 << 7))
	{
		int nBytes = 1;
		for(int idx = 0; idx != 6; ++idx)
		{
			if(ch & (1 << (6 - idx)))
			{
				++nBytes;	
			}
			else
				break;
		}
		return nBytes;
	}
	return 1;
}  
  

std::size_t MyTask::length(const std::string &str)
{
	std::size_t ilen = 0;
	for(std::size_t idx = 0; idx != str.size(); ++idx)
	{
		int nBytes = nBytesCode(str[idx]);
		idx += (nBytes - 1);
		++ilen;
	}
	return ilen;
}


int MyTask::editDistance(const std::string & lhs, const std::string &rhs)
{//计算最小编辑距离-包括处理中英文
	size_t lhs_len = length(lhs);
	size_t rhs_len = length(rhs);
	int editDist[lhs_len + 1][rhs_len + 1];
	for(size_t idx = 0; idx <= lhs_len; ++idx)
	{
		editDist[idx][0] = idx;
	}

	for(size_t idx = 0; idx <= rhs_len; ++idx)
	{
		editDist[0][idx] = idx;
	}
	
	std::string sublhs, subrhs;
	for(std::size_t dist_i = 1, lhs_idx = 0; dist_i <= lhs_len; ++dist_i, ++lhs_idx)
	{
		size_t nBytes = nBytesCode(lhs[lhs_idx]);
        //printf("pos=%ld$size=%ld$\n", lhs_idx,lhs.size());
		sublhs = lhs.substr(lhs_idx, nBytes);
        //printf("pos=%ld$size=%ld$\n", lhs_idx,lhs.size());
		lhs_idx += (nBytes - 1);

		for(std::size_t dist_j = 1, rhs_idx = 0; dist_j <= rhs_len; ++dist_j, ++rhs_idx)
		{
			nBytes = nBytesCode(rhs[rhs_idx]);
			subrhs = rhs.substr(rhs_idx, nBytes);
			rhs_idx += (nBytes - 1);
			if(sublhs == subrhs)
			{
				editDist[dist_i][dist_j] = editDist[dist_i - 1][dist_j - 1];
			}
			else
			{
				editDist[dist_i][dist_j] = triple_min(
					editDist[dist_i][dist_j - 1] + 1,
					editDist[dist_i - 1][dist_j] + 1,
					editDist[dist_i - 1][dist_j - 1] + 1);
			}
		}
	}
	return editDist[lhs_len][rhs_len];
}















}//wd
