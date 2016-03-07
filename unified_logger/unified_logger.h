#ifndef UNIFIED_LOGGER_H
#define UNIFIED_LOGGER_H

#include <cstdlib>
#include <iostream>
#include <thread>
#include <string>
#include <stdio.h>
#include <sys/time.h>
#include <fstream>
#include <sstream>

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
    Unified_logger(bool stdout_logging, std::string log_folder_path, std::string data_tag, int log_level);
    ~Unified_logger();
    //Minimum user input with default values
    Unified_logger(std::string data_tag);

    //Main logging function
    void log(int level, std::string);

    // gets literal representation from MACRO (int) level
    std::string levelToString(int level);

    //get actual time
    //Format: [7.3.2016 21:54:59:382]
    std::string getTime(void);

    //Sets and opens log file
    void setLogFile(std::string path);

    //Sets path to a log file
    void setLogFilePath();

    //Sets log level, all logs below this level are ignored
    void setLogLevel(int log_level);

    //gets filename from date
    //Format: [tag]yymmdd.log
    std::string getFileNameByDate();

private:
    //true when all output is logged to stdout
    bool _stdout_logging;

    //Paths to logs
    std::string _log_folder_path;
    std::string _log_file_path;
    
    //Current log file
    std::ofstream _logfile;

    //Data tag used to determine log source
    std::string _data_tag;
    
    //minimum level to collect logs
    int _log_level;
};

#endif