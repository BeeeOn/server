/* 
 * File:   Logger.cpp
 * Author: pavel
 * 
 * Created on 2. červen 2014, 10:36
 */

#include "Logger.h"

using namespace std;

Logger::Logger() {
    _verbosityThreshold = 0;
    _verbosityLevel = 0;
    _colored = false;
}



Logger::~Logger() {
}

Logger& Logger::getInstance()
{
    static Logger instance;
    return instance;
}

Logger& Logger::getInstance(int newVerbosityLvl)
{
    Logger& l =Logger::getInstance();
    l.setVerbosityLevel(newVerbosityLvl);
    l.printTime();
    return l;
}


void Logger::setVerbosityThreshold(int verbosityThreshold){
    _verbosityThreshold = verbosityThreshold;
}

void Logger::setVerbosityLevel(int verbosityLvl){
    _verbosityLevel = verbosityLvl;
}
void Logger::printTime(){
    if(_verbosityLevel > _verbosityThreshold)
        return;
    timeval tp;
    time_t curtime;
    
    gettimeofday(&tp, 0);
    
    curtime = tp.tv_sec;
    tm *t = localtime(&curtime);
    
    if(_colored)
		cout<<zkr::cc::bold;
    
    cout<<">"<<_verbosityLevel<<"< " << std::this_thread::get_id() <<": ";
        
    printf("%d.%d.%d %02d:%02d:%02d:%03ld ", t->tm_mday,t->tm_mon,(t->tm_year+1900),t->tm_hour, t->tm_min, t->tm_sec, tp.tv_usec/1000);
    if(_colored)
		cout<<zkr::cc::console;
}
/*
Logger& Logger::operator<<(const string& s)
{
    print(s);
    return *this;
}

Logger& Logger::operator<<(const int& i)
{
    stringstream ss;
    ss << i;
    string str = ss.str();
    print(str);
 return *this;
 }
 -*/


Logger &Logger::operator<<(std::ostream& (*pf) (std::ostream&)){
    if(_verbosityLevel > _verbosityThreshold)
        return *this;   
    cout<<pf;
    return *this;
}
