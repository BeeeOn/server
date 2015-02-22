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
    
#ifdef LOGS_TO_STDOUT
    _fileName = "/dev/stdout" ;
#else
    _fileName =  getFileNamebyDate(); 
#endif
    
    _currentFile.open ( _fileName );  
/*
std::ofstream of;
std::ostream& out = condition ? std::cout : of.open(filename);
*/

}

Logger::~Logger() {
    _currentFile.close();
}

void Logger::changeFiles() {
    _currentFile.close();
    
 #ifdef LOGS_TO_STDOUT
    _fileName = "/dev/stdout" ;
#else
    _fileName =  getFileNamebyDate(); 
#endif
    
    _currentFile.open ( _fileName );    
}

string Logger::getFileNamebyDate() {
    
    timeval tp;
    time_t curtime;
    
    gettimeofday(&tp, 0);
    
    curtime = tp.tv_sec;
    tm *t = localtime(&curtime);
    
    ostringstream convert; 
    
    _dayPrecidingChanges = t->tm_mday;
            
    convert<<LOGS_FILE << "/" << "log"<<t->tm_mday <<"_"<< t->tm_mon+1 <<"_"<< (t->tm_year+1900);
    
    return convert.str();
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
    
   
    std::lock_guard<std::mutex> lck (_mtx);
    timeval tp;
    time_t curtime;

    gettimeofday(&tp, 0);

    curtime = tp.tv_sec;
    tm *t = localtime(&curtime);
    
    if(_dayPrecidingChanges != t->tm_mday)
        changeFiles();
    
    #ifdef COLORED_LOGGER
        cout<<zkr::cc::bold;
    #endif 
        
    _currentFile<<">"<<_verbosityLevel<<"< " << std::this_thread::get_id() <<": ";

    //printf("%d.%d.%d %02d:%02d:%02d:%03ld ", t->tm_mday,(t->tm_mon+1),(t->tm_year+1900),t->tm_hour, t->tm_min, t->tm_sec, tp.tv_usec/1000);
    _currentFile << t->tm_mday <<"." << (t->tm_mon+1) << "." << (t->tm_year+1900) <<" ";
    _currentFile << t->tm_hour << ":" << t->tm_min << ":" << t->tm_sec << ":" << tp.tv_usec/1000<< " ";
    
    #ifdef COLORED_LOGGER
            cout<<zkr::cc::console;
    #endif
  
}


Logger &Logger::operator<<(std::ostream& (*pf) (std::ostream&)){
    if(_verbosityLevel > _verbosityThreshold)
        return *this;   
    
    
    std::lock_guard<std::mutex> lck (_mtx);
    _currentFile<<pf;

    if(_verbosityLevel == this->FATAL)
            std::cerr << pf;
    
    return *this;
}
