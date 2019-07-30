#pragma once
#include "SplitTool.h"
#include "cppjieba/Jieba.hpp"

namespace wd
{

const char* const DICT_PATH = "../conf/dict/jieba.dict.utf8";
const char* const HMM_PATH = "../conf/dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "../conf/dict/user.dict.utf8";
const char* const IDF_PATH = "../conf/dict/idf.utf8";                                                                                                                                         
const char* const STOP_WORD_PATH = "../conf/dict/stop_words.utf8";

class CppJieba :
    public SplitTool
{
public:
    CppJieba() : m_jieba(DICT_PATH,
                HMM_PATH,
                USER_DICT_PATH,
                IDF_PATH,
                STOP_WORD_PATH)
    {}
    ~CppJieba() {}
    std::vector<std::string> cut(const std::string & sentence) override
    {
        std::vector<std::string> words;
        m_jieba.Cut(sentence, words, true);
        return words;
    }
private:
    cppjieba::Jieba m_jieba;
};




}//end of namespace wd

