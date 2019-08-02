#pragma once


namespace wd
{

class Noncopyable
{
protected:
    Noncopyable() {}
    ~Noncopyable() {}
    
    Noncopyable(const Noncopyable&) = delete;
    Noncopyable& operator=(const Noncopyable&) = delete;
private:

};



}//end of namespace wd
