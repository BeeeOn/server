#ifndef LOGGER_H
#define	LOGGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <thread>
#include <mutex>
#include "ccolor.h"

//#define  COLORED_LOGGER
#define LOGS_FILE "logs"
//#define LOGS_TO_STDOUT


class Logger {
private:
    Logger();
    Logger(Logger const&);              // Don't Implement
    void operator=(Logger const&); // Don't implement
public:
    virtual ~Logger();
    
    static Logger& getInstance();
    static Logger& getInstance(int newVerbosityLvl);
    void setVerbosityThreshold(int verbosityThreshold);
    
    void changeFiles();
    std::string getFileNamebyDate();
    std::string getFileName();
        int test;
private:
    void setVerbosityLevel(int verbosityLvl);
    void printTime();
    //ofstream _output;
    bool _colored;
    int _cerrVerbosity;
    std::ofstream _currentFile;
    std::string _fileName;
    std::mutex _mtx;
    //day when was stored last log, used for determining new day and change log file
    int _dayPrecidingChanges;
    
public:
    int _verbosityThreshold;
    int _verbosityLevel;
public:
    static const int NO_OUTPUT = 0;
    
    static const int FATAL = 1;
    
    static const int ERROR = 3;
    
    static const int DEBUG = 5;
    static const int DEBUG2 = 6;
    static const int DEBUG3 = 7;
    
    //allow to use function like std::endl
    Logger &operator<<(std::ostream& (*pf) (std::ostream&));
    
    template <typename T>
    Logger &operator<<(const T &a) {
        
        if(_verbosityLevel > _verbosityThreshold)
            return *this;
        
        
        std::lock_guard<std::mutex> lck (_mtx);
        
        if(_verbosityLevel <= _cerrVerbosity)
        
        #ifdef COLORED_LOGGER
        if(_verbosityLevel == this->FATAL)
            _currentFile<<zkr::cc::fore::red << zkr::cc::bold;
        
        if(_verbosityLevel == this->ERROR)
            _currentFile<<zkr::cc::bold;
        #endif

        if(_verbosityLevel  <= _cerrVerbosity)
            std::cerr << a;
       _currentFile<<a;
       
        #ifdef COLORED_LOGGER
            std::cout<<zkr::cc::console;
       #endif

        _currentFile<<std::flush;
        
        return *this;
    }
};

#endif	/* LOGGER_H */
