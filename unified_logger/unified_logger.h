#ifndef UNIFIED_LOGGER_H
#define UNIFIED_LOGGER_H

/**
 * unified_logger.h
 * Library used for logging in unified format in multi-thread applications
 * @author Marek Beňo, xbenom01 at stud.fit.vutbr.cz
 * 7. April 2016
 */  

#include <unistd.h>
#include <cstdlib>
#include <stdio.h>
#include <string>

#include <iostream>
#include <fstream>
#include <sstream>

#include <mutex>
#include <thread>
#include <vector>

#include "locked_stream.h"


#define LOGOUT(tag, severity) \
    out(__FILE__, __LINE__, tag, severity) 

#define LOGFILE(tag, severity) \
    file(__FILE__, __LINE__, tag, severity) 

enum class LogSeverity {ALL, TRACE, DEBUG, MSG, WARN, INFO, ERROR, FATAL, NONE};


/**
 * @brief Library used for logging in unified format in multi-thread applications
 * @details demo usage in unified_logger_demo.cpp
 */
class Unified_logger
{
public:
    Unified_logger(std::string logger_id, std::string log_folder_path = ".", LogSeverity default_log_level = LogSeverity::WARN);
    ~Unified_logger();

    //Note: use macro  LOGOUT to avoid having to manually insert first two parameters
    locked_stream out(std::string location, int line, std::string tag, std::string level);

    //Note: use macro  LOGFILE to avoid having to manually insert first two parameters
    locked_stream file(std::string location, int line, std::string tag, std::string level);

    //Sets log level, all logs below this level are ignored
    void setLogLevel(LogSeverity log_level);

    void setLogFolderPath(std::string folder_path);

    //returns path to output file
    std::string getLogFilePath();

    //return tagname for log Format: beeeon.<hostname>.<_logger_id>.<tag>
    std::string getTagHierarchy(std::string tag);

    // gets literal representation from LogSeverity level
    std::string levelToString(LogSeverity level);

    //gets internal representation from log level
    LogSeverity StringToLevel(std::string severity);

private:
    //true when all output is logged to stdout
    bool _log_to_stdout;

    //Paths to logs
    std::string _log_folder_path;

    std::string _log_file_path;

    //logfile for output to file    
    std::ofstream _logfile;

    // file /dev/null for filtration
    std::ofstream _filtered;

    //representation of logger, should correspond to unit in BeeeOn system
    std::string _logger_id;

    //minimum level to collect logs
    LogSeverity _minimum_log_level;


};



#endif