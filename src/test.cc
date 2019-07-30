#include "../include/SpellcorrectServer.h"
#include "../include/MyDict.h"
#include "../include/MyConf.h"


int main()
{
    //传入配置文件的路径, 初始化词典
    wd::MyConf* pConf = wd::MyConf::getInstance("../conf/config.utf8");
    
    //pConf->showConfig();//test

    wd::MyDict::getInstance(pConf->getConfig()["en_dict"], 
                            pConf->getConfig()["cn_dict"],
                            pConf->getConfig()["indexTable"]);
    printf("success\n");
    wd::SpellcorrectServer scServer(*pConf);
    scServer.start();
    return 0;
}

