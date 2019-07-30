#pragma once
#include "tinyxml2.h"
#include<iostream>
#include<string>
#include<vector>
using std::vector;
using std::string;

struct RssItem
{
    string title;
    string link;
    string description;
    string content;
};

class RssReader
{
public:
    RssReader() {}
    ~RssReader() {}
    void parseRssItem(tinyxml2::XMLElement* elem);
    void parseRss(const char* xmlName);//解析
    void dump(const string &fileName);//输出至pagelib.txt
private:
    vector<RssItem> m_vRss;
};

