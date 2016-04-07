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

enum class LogSeverity {ALL, TRACE, DEBUG, WARN, INFO, ERROR, FATAL, NONE};

class Unified_logger
{
public:
    Unified_logger(std::string logger_id, std::string log_folder_path = ".", LogSeverity default_log_level = LogSeverity::WARN);
    ~Unified_logger();

    //logging function
    //void log(int level, std::string);

    locked_stream out(std::string location, int line, std::string tag, std::string level);

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

    //logfile for output to file    
    std::ofstream _logfile;

    // file /dev/null for filtration
    std::ofstream _filtered;

    //representation of logger, should correspond to unit in BeeeOn system
    std::string _logger_id;

    //minimum level to collect logs
    LogSeverity _default_log_level;


};

#endif