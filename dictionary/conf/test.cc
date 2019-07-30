#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::string;
using std::ifstream;
int main()
{
    ifstream ifs("./C3-Art0023.txt");
    string str;
    while(ifs >> str)
        cout << str << endl;
    return 0;
}

