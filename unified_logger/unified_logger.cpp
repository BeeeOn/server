#include "unified_logger.h"

Unified_logger::Unified_logger(bool log_to_stdout, std::string log_folder_path, std::string logger_id, int default_log_level):

    _log_to_stdout(log_to_stdout),
    _log_folder_path(log_folder_path),
    _logger_id(logger_id),
    _default_log_level(default_log_level)
{
};

//Default values
Unified_logger::Unified_logger(std::string logger_id):
    _log_to_stdout(true),
    _log_folder_path("."),
    _logger_id(logger_id),
    _default_log_level(ERROR)
{
};

/*
Unified_logger::~Unified_logger()
{
}*/


std::unique_ptr<shard> Unified_logger::getShard(std::string id, int log_level)
{
    std::unique_ptr<shard> tmp(new shard(id, log_level, this->_log_to_stdout, 
        this->_log_folder_path, this->getLogFilePath(id), this->_logger_id));
    return tmp;
}

std::unique_ptr<shard> Unified_logger::getShard(std::string id)
{
    std::unique_ptr<shard> tmp(new shard(id, this->_default_log_level, this->_log_to_stdout, 
        this->_log_folder_path, this->getLogFilePath(id), this->_logger_id));
    return tmp;
}

/**
 * @brief Set minimum level to collect logs
 * 
 * @param log_level int value 1-6
 */
void Unified_logger::setLogLevel(int log_level)
{
    _default_log_level = log_level;
}

void Unified_logger::setLogFolderPath(std::string folder_path)
{
    _log_folder_path = folder_path;
}

/**
* @brief Get inner string for log file path
*/
std::string Unified_logger::getLogFilePath(std::string tag)
{
    return (_log_folder_path + "/" + tag + getFileNameByDate() + ".log");
}

/**
 * @brief Log message function
 * @details Outputs given message
 * 
 * @param level log level
 * @param message log data
 */
/*void Unified_logger::log(int level, std::string message)
{   
    if(level >= _log_level)
    {
        if(_log_to_stdout)
        {
            std::cout << getTime() << " " << levelToString(level) << " " << _data_tag << " " << message << std::endl;
        }
        else
        {
            _logfile << getTime() << " " << levelToString(level) << " " << _data_tag << " " << message << std::endl;
        }
    }
}
*/

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

shard::shard(std::string tag, int log_level, bool log_to_stdout , std::string log_folder_path, std::string log_file_path, std::string parent_logger_id):
_tag(tag),
_log_level(log_level),
_log_to_stdout(log_to_stdout),
_log_folder_path(log_folder_path),
_log_file_path(log_file_path),
_parent_logger_id(parent_logger_id)
{
    if(!_log_to_stdout)
    {
        _logfile.open(_log_file_path, std::ios::app);
    }
}

void shard::log(int level, std::string message)
{   
    std::cout << level << " " << _tag << " " << message << std::endl;
}

void shard::setTag(std::string tag)
{
    _tag = tag;
}

void shard::setLogLevel(int log_level)
{
    _log_level = log_level;
}

std::string shard::getLogFilePath(std::string tag)
{
    return (_log_folder_path + "/" + tag + getFileNameByDate() + ".log");
}

void shard::setLogFilePath(std::string log_file_path)
{
    _log_file_path = log_file_path;
}

//Sets and opens log file
void shard::setLogFile(std::string path)
{

}