#pragma once
#include <vector>
#include <string>


namespace wd
{

class SplitTool
{
public:
    SplitTool() {}
    virtual ~SplitTool() {}
    virtual std::vector<std::string> cut(const std::string & sentence) = 0;
};







}//end of namespace wd

