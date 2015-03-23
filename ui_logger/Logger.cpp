/* 
 * File:   Logger.cpp
 * Author: pavel
 * 
 * Created on 2. červen 2014, 10:36
 */

#include "Logger.h"

using namespace std;

Logger::Logger() {
    _verbose = Logger::DEBUG3;
    _level = Logger::NO_OUTPUT;
    _cerrVerbosity = Logger::NO_OUTPUT; 
    _fileName =  getFileName(); 
    
    _currentFile.open ( _fileName );  

}

Logger::~Logger() {
    _currentFile.close();
}


Logger& Logger::getInstance()
{
    static Logger instance;
    return instance;
}

Logger& Logger::getInstance(int level)
{
    Logger& l =Logger::getInstance();
    l.setLevel(level);
    l.printTime();
    return l;
}

Logger& Logger::debug()
{
    Logger& l =Logger::getInstance(Logger::DEBUG);
    return l;
}

Logger& Logger::error()
{
    Logger& l =Logger::getInstance(Logger::ERROR);
    return l;
}

Logger& Logger::fatal()
{
    Logger& l =Logger::getInstance(Logger::FATAL);
    return l;
}

Logger& Logger::db()
{
    Logger& l =Logger::getInstance(Logger::DB);
    return l;
}

void Logger::openOutput(){
/*
   #ifdef LOGS_TO_STDOUT
	_output = std:cout;
   #else
	_currentFile.open
	_output = _currentFile;	
*/
}

void Logger::changeFiles() {
    _currentFile.close();
    
    _fileName =  getFileName(); 
    
    _currentFile.open ( _fileName );    
}

std::string Logger::getFileName() {
    
    #ifdef LOGS_TO_STDOUT
       _fileName = "/dev/stdout" ;
   #else
       _fileName =  getFileNamebyDate(); 
   #endif
       return _fileName;
}

string Logger::getFileNamebyDate() {
    
    timeval tp;
    time_t curtime;
    
    gettimeofday(&tp, 0);
    
    curtime = tp.tv_sec;
    tm *t = localtime(&curtime);
    
    ostringstream convert; 
    
    _dayOfLastLog = t->tm_mday;
            
    convert<<LOGS_FILE << "/" << "log"<<t->tm_mday <<"_"<< t->tm_mon+1 <<"_"<< (t->tm_year+1900);
    
    return convert.str();
}

void Logger::setVerbose(int verbose){
    _verbose = verbose;
}

void Logger::setLevel(int level){
    _level = level;
}
void Logger::printTime(){   
    if(_level > _verbose)
        return;
    
   
    std::lock_guard<std::mutex> lck (_mtx);
    timeval tp;
    time_t curtime;

    gettimeofday(&tp, 0);

    curtime = tp.tv_sec;
    tm *t = localtime(&curtime);
    
    if(_dayOfLastLog != t->tm_mday)
        changeFiles();
            
    _currentFile<<">"<<_level<<"< " << std::this_thread::get_id() <<": ";

    //printf("%d.%d.%d %02d:%02d:%02d:%03ld ", t->tm_mday,(t->tm_mon+1),(t->tm_year+1900),t->tm_hour, t->tm_min, t->tm_sec, tp.tv_usec/1000);
    _currentFile << t->tm_mday <<"." << (t->tm_mon+1) << "." << (t->tm_year+1900) <<" ";
    _currentFile << t->tm_hour << ":" << t->tm_min << ":" << t->tm_sec << ":" << tp.tv_usec/1000<< " ";
    
        if(_level <= _cerrVerbosity){
            cerr<<">"<<_level<<"< " << std::this_thread::get_id() <<": ";
            cerr << t->tm_mday <<"." << (t->tm_mon+1) << "." << (t->tm_year+1900) <<" ";
            cerr<< t->tm_hour << ":" << t->tm_min << ":" << t->tm_sec << ":" << tp.tv_usec/1000<< " ";            
        }
    
  
}


Logger &Logger::operator<<(std::ostream& (*pf) (std::ostream&)){
    if(_level > _verbose)
        return *this;   
    
    
    std::lock_guard<std::mutex> lck (_mtx);
    _currentFile<<pf;

    if(_level <= _cerrVerbosity)
            std::cerr << pf;
    
    return *this;
}
