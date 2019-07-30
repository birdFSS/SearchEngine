#pragma once
#include "Configuration.h"
#include "Mylog.h"
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#include <string>
using std::string;

namespace wd
{

class DirScanner
{
public:
    DirScanner(Configuration& conf) : 
        m_conf(conf)
    {}
    ~DirScanner() {}

    void operator()()
    {
       auto dirName = getAbosolutePath(m_conf.getConfigMap().at("sourceDirName"));
       travers(dirName);        //语料库主目录
    }

    std::vector<std::string>& getFilePaths() { return m_filePaths; }

    void travers(const std::string& dirname)
    {
        DIR* dir = opendir(dirname.c_str());
        if(nullptr == dir)
        {
            logError("opendir error");
        }

        struct dirent *d;
        string filePath;
        while((d = readdir(dir)) != nullptr)
        {
            if(d->d_type & DT_REG)                      //需要可以加个判断，判断文件尾缀
            {
                filePath = dirname + "/" + d->d_name;
                m_filePaths.push_back(std::move(filePath));
            }else if(d->d_type & DT_DIR){
                if(d->d_name != string(".") && d->d_name != string(".."))
                {
                    travers(dirname + "/" + d->d_name);     //递归遍历子文件夹内容
                }
            }
        }

        if(closedir(dir) == -1)
        {
            logError("closedir error");
            return ;
        }
    }//void travers()
    
    void show() const   //test
    {
        for(const auto& i : m_filePaths)
        {
            logInfo(i.c_str());
        }
    }
private:
    std::string getAbosolutePath(const std::string & fileName)
    {
        char buff[2048] = {0};
        if(realpath(fileName.c_str(), buff))
        {
            return std::string(buff);
        }else{
            logError("realpath");
            return std::string("");
        }   
    }
private:
    Configuration& m_conf;
    std::vector<std::string> m_filePaths;
};







}//end of namespace wd

