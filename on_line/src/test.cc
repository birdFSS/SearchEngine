#include "../include/WordQueryServer.h"


int main()
{
    //传入配置文件的路径, 初始化词典
    wd::WordQueryServer wqs("../conf/myconf.conf");
    wqs.start();
    return 0;
}

