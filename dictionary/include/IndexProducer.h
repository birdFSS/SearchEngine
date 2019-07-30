#pragma once

#include <vector>
#include <map>
#include <set>
#include <string>
#include <iostream>
namespace wd
{
class DictProducer;
class IndexProducer
{
public:
    
    void createIndex(const DictProducer& en_dict,const DictProducer& cn_dict, std::ostream & os);
private:
    //std::vector<std::pair<std::string,int>> m_index;
    std::map<std::string, std::set<int>> m_index;
};







}//end of namespace wd

