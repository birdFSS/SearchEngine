#include "../include/RssReader.h"
#include "../include/tinyxml2.h"
#include "../include/Mylog.h"
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
    if(nullptr == elem)
    {
        logError("elem is nullptr"); 
        return;
    }

    XMLElement* channel;
    if((channel = elem->FirstChildElement("channel")) == nullptr)
    {
        logError("channel is nullptr");
        return;
    }

    XMLElement* item ;
    if((item = channel->FirstChildElement("item")) == nullptr)
    {
        logError("item is nullptr");
        return ;
    }

    do{
        //cout << "get in RssReader::parseRssItem(tinyxml2::XMLElement* elem)" << endl;
        RssItem rssTmp;
        if(item->FirstChildElement("link") != nullptr && item->FirstChildElement("link")->GetText() != nullptr)
        {
            //logInfo("...");
                rssTmp.link = item->FirstChildElement("link")->GetText();
        }else{
            continue;
        }

        if(item->FirstChildElement("content:encoded") && item->FirstChildElement("content:encoded")->GetText())
        {
            rssTmp.content = item->FirstChildElement("content:encoded")->GetText();
        }else if(item->FirstChildElement("description") && item->FirstChildElement("description")->GetText()){
            rssTmp.content= item->FirstChildElement("description")->GetText();
        }else{
            continue;
        }

        boost::regex pat("<[^>]*>");
        rssTmp.content = boost::regex_replace(rssTmp.content, pat, " ");
#if 0           //暂时用不到description
        if(item->FirstChildElement("description"))
        {
            rssTmp.description = item->FirstChildElement("description")->GetText();
        }
#endif

        if(item->FirstChildElement("title") && item->FirstChildElement("title")->GetText())
        {
            rssTmp.title = item->FirstChildElement("title")->GetText();
        }else{
            if(rssTmp.content.size() > 100)
            {
                rssTmp.title.assign(rssTmp.content, 0, 96);
            }else{
                rssTmp.title.assign(rssTmp.content, 0);
            }
        }
        //logInfo("%s", rssTmp.title.c_str());
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

void RssReader::dump(const string& fileName, const string& offsetFile)
{
    //ofstream ofs_doc(fileName, std::ios::app); 
    //ofstream ofs_offset(offsetFile, std::ios::app);
    ofstream ofs_doc(fileName);
    ofstream ofs_offset(offsetFile);
    string doc_str;
    string offset_str;
    for(size_t i=0;i != m_vRss.size();i++)
    {
        doc_str = "<doc>\n\t<docid>" + to_string(i+1) + "</docid>\n\t<url>" 
                  + m_vRss[i].link + "</url>\n\t<title>"
                  + m_vRss[i].title + "</title>\n\t<content>"
                  + m_vRss[i].content + "\n</content>\n</doc>\n";

        offset_str = to_string(i+1) + " " + to_string(ofs_doc.tellp()) + " "
                    + to_string(doc_str.size()) + "\n";
        
        ofs_doc << doc_str;
        ofs_offset << offset_str;
    }
}


