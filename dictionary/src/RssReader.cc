#include "../include/RssReader.h"
#include "../include/tinyxml2.h"
#include <boost/regex.hpp>
#include <fstream>
#include <string>

using namespace tinyxml2;
using std::to_string;
using std::ofstream;
using std::cout;
using std::endl;
using std::string;

void RssReader::parseRssItem(tinyxml2::XMLElement* elem)
{
    XMLElement* channel = elem->FirstChildElement("channel");
    XMLElement* item = channel->FirstChildElement("item");
    do{
        //cout << "get in RssReader::parseRssItem(tinyxml2::XMLElement* elem)" << endl;
        RssItem rssTmp;
        rssTmp.title = item->FirstChildElement("title")->GetText();
        rssTmp.link = item->FirstChildElement("link")->GetText();
        rssTmp.description = item->FirstChildElement("description")->GetText();
        string tmp = item->FirstChildElement("content:encoded")->GetText();
        boost::regex pat("<[^>]*>");
        rssTmp.content = boost::regex_replace(tmp, pat, " ");
        //cout << rssTmp.content << endl;
        m_vRss.push_back(rssTmp);
    }while((item = item->NextSiblingElement("item")) != NULL);
}

/*将doc读出来的分别归类*/
void RssReader::parseRss(const char* xmlName)
{
    XMLDocument doc;
    if(doc.LoadFile(xmlName))
    {
        cout << "error loadFile" << endl;
        return;
    }
    XMLElement *root = doc.RootElement();
    parseRssItem(root);
}

void RssReader::dump(const string& fileName)
{
    ofstream ofs(fileName); 
    for(size_t i=0;i != m_vRss.size();i++)
    {
        string str = "<doc>\n\t<docid>" + to_string(i+1) + "</docid>\n\t<title>" + m_vRss[i].link + "</link>\n\t<content>"
               + m_vRss[i].content + "</content>\n</doc>\n";
        ofs << str;
#if 0
        ofs << "<doc>\n" ;
        ofs << "<docid>" << i+1 << "</docid>\n";
        ofs << "<title>" << m_vRss[i].title << "</title>\n";
        ofs << "<link>" << m_vRss[i].link << "</link>\n";
        //ofs << "<description>" << m_vRss[i].description << "</description>\n";
        ofs << "<content>" << m_vRss[i].content << "</content>\n";
        ofs << "</doc>\n";
#endif
    }
}


