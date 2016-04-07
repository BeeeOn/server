#ifndef UNIFIED_LOGGER_H
#define UNIFIED_LOGGER_H

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

//Log levels
#define TRACE 1
#define DEBUG 2
#define WARN 3
#define INFO 4
#define ERROR 5
#define FATAL 6
#define NONE 1024

/*
class shard
{
public:
    shard(std::string id);

    shard(std::string tag, int log_level, bool log_to_stdout , std::string log_folder_path, 
        std::string log_file_path, std::string parent_logger_id);

    void log(int level, std::string message);

    locked_stream out(std::string tag);

    void setTag(std::string tag);

    void setLogLevel(int log_level);

    std::string getLogFilePath(std::string tag);

    void setLogFilePath(std::string log_file_path);

    //Sets and opens log file
    void setLogFile(std::string path);

    //locked_stream& operator<<();

    //Data tag used to determine log source
    std::string _tag;

private:



    //minimum level to collect logs
    int _log_level;

    bool _log_to_stdout;

    //Path to logs
    std::string _log_folder_path;
    std::string _log_file_path;

    //Current log file
    std::ofstream _logfile;

    std::string _parent_logger_id;


};*/

class Unified_logger
{
public:
    Unified_logger(bool stdout_logging, std::string log_folder_path, std::string data_tag, int log_level);
    //~Unified_logger();
    //Minimum user input with default values
    Unified_logger(std::string data_tag);

    //Main logging function
    //void log(int level, std::string);

    //shard* getShard(std::string id, int log_level);
    //shard* getShard();

    locked_stream out(std::string location, int line, std::string tag, std::string level);

    void setOutLogging(bool log_to_stdout);

    //Sets log level, all logs below this level are ignored
    void setLogLevel(int log_level);

    void setLogFolderPath(std::string folder_path);

    std::string getLogFilePath(std::string tag);

    // gets literal representation from MACRO (int) level
    std::string levelToString(int level);



private:
    //true when all output is logged to stdout
    bool _log_to_stdout;

    //Paths to logs
    std::string _log_folder_path;
    
    std::string _logger_id;

    //minimum level to collect logs
    int _default_log_level;


};

#endif