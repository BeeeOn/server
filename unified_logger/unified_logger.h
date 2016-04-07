#ifndef UNIFIED_LOGGER_H
#define UNIFIED_LOGGER_H

//#include <stdlib.h>
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <string>
#include <stdio.h>

#include <fstream>
#include <sstream>

#include <mutex>
#include <thread>
#include <vector>

#include "locked_stream.h"
#include "utility.h"

#define LOGOUT(tag, severity) \
    out(__FILE__, __LINE__, tag, severity) 

#define LOGFILE(tag, severity) \
    file(__FILE__, __LINE__, tag, severity) 

//Log levels
#define TRACE 1
#define DEBUG 2
#define WARN 3
#define INFO 4
#define ERROR 5
#define FATAL 6
#define NONE 1024



class Unified_logger
{
public:
    Unified_logger(std::string log_folder_path, std::string logger_id, int default_log_level);
    //~Unified_logger();
    //Minimum user input with default values
    Unified_logger(std::string logger_id);

    //logging function
    //void log(int level, std::string);


    locked_stream out(std::string location, int line, std::string tag, std::string level);

    locked_stream file(std::string location, int line, std::string tag, std::string level);

    void setOutLogging(bool log_to_stdout);

    //Sets log level, all logs below this level are ignored
    void setLogLevel(int log_level);

    void setLogFolderPath(std::string folder_path);

    std::string getLogFilePath();

    std::string getTagHierarchy(std::string tag);

    // gets literal representation from MACRO (int) level
    std::string levelToString(int level);



private:
    //true when all output is logged to stdout
    bool _log_to_stdout;

    //Paths to logs
    std::string _log_folder_path;
    
    std::ofstream _logfile;

    std::string _logger_id;

    //minimum level to collect logs
    int _default_log_level;


};

#endif