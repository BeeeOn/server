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

#define DEFAULT_LOGS_FILE "logs"
#define LOGS_TO_STDOUT


class Logger {
private:
    Logger();
    Logger(Logger const&);              // Don't Implement
    void operator=(Logger const&); // Don't implement
public:
    virtual ~Logger();
    
    static Logger& getInstance();
    static Logger& getInstance(int newVerbosityLvl);
    static Logger& debug();
    static Logger& debug2();
    static Logger& debug3();
    static Logger& error();
    static Logger& fatal();
    static Logger& db();
    
    void setOutputToFiles(std::string folder=DEFAULT_LOGS_FILE);
    void setOutputToStdout();
    void setVerbose(int verbose);
    void setCerrVerbose(int verbose);

    bool isOutputSetToCout();
    
    void openOutput(std::string fileName);    
    void changeFiles();
    std::string getFileNamebyDate();
    std::string getFileName();

private:
    void setLevel(int level);
    void printTime();

    int _cerrVerbosity;
    std::ostream& _output;	
    //std::streambuf *_stdoutBackup;
    std::ofstream _currentFile;
    std::string _logsFolder;
    std::string _fileName;
    std::mutex _mtx;
    bool _outputToStdout;
    
    int _day;
    
public:
    int _verbose;
    int _level;
public:
    static const int NO_OUTPUT = 0;
    
    static const int FATAL = 1;
    
    static const int ERROR = 3;
    
    static const int DEBUG = 5;
    static const int DEBUG2 = 6;
    static const int DB = 7;
    static const int DEBUG3 = 7;
    
    //allow to use function like std::endl
    Logger &operator<<(std::ostream& (*pf) (std::ostream&));
    
    template <typename T>
    Logger &operator<<(const T &a) {
        
        if(_level > _verbose)
            return *this;
        std::lock_guard<std::mutex> lck (_mtx);
        
        if(_level  <= _cerrVerbosity)
            std::cerr << a;
       _output<<a;

        _output<<std::flush;
        
        return *this;
    }
};

#endif	/* LOGGER_H */
