#include "Logger.h"

using namespace std;

Logger::Logger(): _output(std::cout) {
    _outputToStdout = true;
    _mtx.unlock();
    _verbose = Logger::DEBUG3;
    _level = Logger::NO_OUTPUT;
    _cerrVerbosity = Logger::NO_OUTPUT; 
    
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
    Logger& l = Logger::getInstance();
    l.setLevel(level);
    l.printTime();
    return l;
}

Logger& Logger::debug()
{
    Logger& l =Logger::getInstance(Logger::DEBUG);
    return l;
}

Logger& Logger::debug2()
{
    Logger& l =Logger::getInstance(Logger::DEBUG2);
    return l;
}

Logger& Logger::debug3()
{
    Logger& l =Logger::getInstance(Logger::DEBUG3);
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

void Logger::setVerbose(int verbose){
    _verbose = verbose;
}
void Logger::setCerrVerbose(int verbose){
    _cerrVerbosity = verbose;
}
void Logger::setLevel(int level){
    _level = level;
}

void Logger::setOutputToFiles(string folder){
    _outputToStdout = false;
    _logsFolder = folder;
    openOutput( getFileName() );
}
 void Logger::setOutputToStdout(){
    if( _currentFile.is_open() )
         _currentFile.close();
    
    if(_outputToStdout == false){ 
        _output.rdbuf( std::cout.rdbuf());
        _outputToStdout = true;
    }
 }
bool Logger::isOutputSetToCout() {
    return _outputToStdout;
}

 
void Logger::openOutput(string fileName) {

    _currentFile.open ( fileName );  
    _output.rdbuf( _currentFile.rdbuf());
}

void Logger::changeFiles() {
    _currentFile.close();
    
    _fileName =  getFileName(); 
    
    openOutput(_fileName);
    
}

std::string Logger::getFileName() {
 
       return getFileNamebyDate();
}

string Logger::getFileNamebyDate() {
    
    timeval tp;
    time_t curtime;
    
    gettimeofday(&tp, 0);
    
    curtime = tp.tv_sec;
    tm *t = localtime(&curtime);
    
    ostringstream convert; 
    
    _day = t->tm_mday;
            
    convert<<_logsFolder << "/" << "log"<<t->tm_mday <<"_"<< t->tm_mon+1 <<"_"<< (t->tm_year+1900);
    
    return convert.str();
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
    
    if(!_outputToStdout){
        if(_day != t->tm_mday)
            changeFiles();
    }       
    
    _output<<"|"<<_level<<"|~" << std::this_thread::get_id() <<":~";

    _output << t->tm_mday <<"." << (t->tm_mon+1) << "." << (t->tm_year+1900) <<"~";
    _output << t->tm_hour << ":" << t->tm_min << ":" << t->tm_sec << ":" << tp.tv_usec/1000<< "~";
    
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
    _output<<pf;

    if(_level <= _cerrVerbosity)
            std::cerr << pf;
    
    return *this;
}
