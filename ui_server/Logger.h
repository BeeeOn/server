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

#include "ccolor.h"


using namespace std;

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
    /*
     Logger& operator<<(const string& s);
     Logger& operator<<(const int& i);
     */
    template <typename T>
    Logger &operator<<(const T &a) {
        
        if(_verbosityLevel > _verbosityThreshold)
            return *this;
        
        if(_verbosityLevel == this->FATAL && _colored)
            cout<<zkr::cc::fore::red << zkr::cc::bold;
        
        if(_verbosityLevel == this->ERROR && _colored)
            cout<<zkr::cc::bold;
        
        cout<<a;
        if(_colored)
            cout<<zkr::cc::console;
        cout<<flush;
        return *this;
    }
    
    //allow to use function like std::endl
    Logger &operator<<(std::ostream& (*pf) (std::ostream&));
    
    int test;
private:
    void setVerbosityLevel(int verbosityLvl);
    void printTime();
    //ofstream _output;
    bool _colored;
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
};

#endif	/* LOGGER_H */
