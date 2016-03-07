#include "unified_logger.h"

Unified_logger::Unified_logger(bool stdout_logging, std::string log_folder_path, std::string data_tag, int log_level):

    _stdout_logging(stdout_logging),
    _log_folder_path(log_folder_path),
    _data_tag(data_tag),
    _log_level(log_level)
{
    if(!_stdout_logging)
    {
        this->setLogFilePath();
        _logfile.open(_log_file_path, std::ios::app);
    }
};

//Default values
Unified_logger::Unified_logger(std::string data_tag):

    _stdout_logging(true),
    _log_folder_path("."),
    _data_tag(data_tag),
    _log_level(ERROR)
{
    if(!_stdout_logging)
    {
        this->setLogFilePath();
        _logfile.open(_log_file_path, std::ios::app);
    }
};

Unified_logger::~Unified_logger()
{
    if(!_stdout_logging)
    {
        _logfile.close();
    }
}

/**
 * @brief Set inner string for log file path
 */
void Unified_logger::setLogFilePath()
{
    _log_file_path = _log_folder_path + "/" + _data_tag + this->getFileNameByDate() + ".log";
}


/**
 * @brief Set minimul level to collect logs
 * 
 * @param log_level int balue 1-6
 */
void Unified_logger::setLogLevel(int log_level)
{
    _log_level = log_level;
}

/**
 * @brief Gets filename from current date
 * @details Log file filename encodes current date in format: yymmdd
 * @return date encoded in string according to format
 */
std::string Unified_logger::getFileNameByDate()
{
    timeval now;
    gettimeofday(&now, 0);
    tm *curr_time = localtime(&now.tv_sec);

    std::ostringstream buff;
    buff << (curr_time->tm_year+1900);
    if ( curr_time->tm_mon+1 < 10) { buff << "0";}
    buff << curr_time->tm_mon+1;
    if ( curr_time->tm_mday < 10) { buff << "0";}
    buff << curr_time->tm_mday;
    return buff.str();
}


/**
 * @brief Gets string representation from int log level
 * 
 * @param level log level MACRO (int 1-6 , 1024)
 * @return string representation of log level
 */
std::string Unified_logger::levelToString(int level)
{
    switch(level)
    {
        case TRACE:
            return "TRACE";
        case DEBUG:
            return "DEBUG";
        case WARN:
            return "WARN ";
        case INFO:
            return "INFO ";
        case ERROR:
            return "ERROR";
        case FATAL:
            return "FATAL";
        case NONE:
            return "NONE ";
        default:
            return "Unknown";
    }
}

/**
 * @brief Log message function
 * @details Outputs given message
 * 
 * @param level log level
 * @param message log data
 */
void Unified_logger::log(int level, std::string message)
{   
    if(level >= _log_level)
    {
        if(_stdout_logging)
        {
            std::cout << this->getTime() << " " << levelToString(level) << " " << _data_tag << " " << message << std::endl;
        }
        else
        {
            _logfile << this->getTime() << " " << levelToString(level) << " " << _data_tag << " " << message << std::endl;
        }
    }
}

/**
 * @brief Gets time udes in logs
 * @return string representation of current date
 * Format: [7.3.2016 21:54:59:382]
 */
std::string Unified_logger::getTime(void)
{
    timeval now;
    gettimeofday(&now, 0);
    tm *curr_time = localtime(&now.tv_sec);

    std::ostringstream buff;

    buff << "[" << curr_time->tm_mday << "." << curr_time->tm_mon+1 << "." << (curr_time->tm_year+1900) << " ";
    if(curr_time->tm_hour<10){buff << "0";}
    buff << curr_time->tm_hour << ":";

    if(curr_time->tm_min<10){buff << "0";}
    buff << curr_time->tm_min << ":";

    if(curr_time->tm_sec<10){buff << "0";}
    buff << curr_time->tm_sec << ":" << now.tv_usec/1000 << "] ";
    return buff.str();
}

