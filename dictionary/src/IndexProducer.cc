#include "../include/IndexProducer.h"
#include "../include/DictProducer.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::string;

namespace wd
{


void IndexProducer::createIndex(const DictProducer& en_dict,const DictProducer& cn_dict, std::ostream &os)
{
    auto iter1 = en_dict.m_dict.begin();
    size_t line_no = 0;

    //遍历每一个词
    while(iter1 != en_dict.m_dict.end())
    {
        //遍历每一个字母
        for(auto it_char = iter1->first.begin();
            it_char != iter1->first.end();
            ++it_char)
        {
            string str(1,0);
            str[0] = *it_char;
            m_index[str].insert(line_no);

        }
        ++line_no;
        ++iter1;
    }

#if 1
    auto iter2 = cn_dict.m_dict.begin();
    // 遍历每一个词
    while(iter2 != cn_dict.m_dict.end())
    {
        //遍历每一个字,每个字占3个字节,避免乱入字符
        for(auto it_cn = iter2->first.begin();
            it_cn != iter2->first.end() &&
            it_cn + 1 != iter2->first.end() &&
            it_cn + 2 != iter2->first.end(); 
            it_cn += 3)
        {
            string str(it_cn,it_cn+3);
            m_index[str].insert(line_no);
        }
        ++line_no;
        ++iter2;
    }
#endif

    auto it_map = m_index.begin();
    while(it_map != m_index.end())
    {
        os << it_map->first << " ";
        for(auto it_set = it_map->second.begin(); it_set != it_map->second.end(); ++ it_set)
        {
            os << *it_set << " ";
        }
        os << endl;
        ++it_map;
    }
}























}
