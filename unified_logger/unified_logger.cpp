#include "unified_logger.h"

/**
 * unified_logger.cpp
 * Library used for logging in unified format in multi-thread applications
 * @author Marek Beňo, xbenom01 at stud.fit.vutbr.cz
 * 7. April 2016
 */  

Unified_logger::Unified_logger(std::string logger_id, std::string log_folder_path, LogSeverity default_log_level):
    _log_folder_path(log_folder_path),
    _logger_id(logger_id),
    _default_log_level(default_log_level)
{
    _logfile.open( getLogFilePath(), std::ios::app);
    _filtered.open( "/dev/null", std::ios::app);
}

Unified_logger::~Unified_logger()
{
    _logfile.close();
    _filtered.close();
}

/**
 * @brief Set minimum level to collect logs
 * 
 * @param log_level int value 1-6
 */
void Unified_logger::setLogLevel(LogSeverity log_level)
{
    _default_log_level = log_level;
}

void Unified_logger::setLogFolderPath(std::string folder_path)
{
    _log_folder_path = folder_path;
}

/**
* @brief Get string representation of path to logfile
*/
std::string Unified_logger::getLogFilePath()
{
    return (_log_folder_path + "/" + _logger_id + getFileNameByDate() + ".log");
}

/**
 * @brief Log message function
 * @details deprecated
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
std::string Unified_logger::levelToString(LogSeverity level)
{
    switch(level)
    {
        case LogSeverity::TRACE:
            return "TRACE";
        case LogSeverity::DEBUG:
            return "DEBUG";
        case LogSeverity::WARN:
            return "WARN ";
        case LogSeverity::INFO:
            return "INFO ";
        case LogSeverity::ERROR:
            return "ERROR";
        case LogSeverity::FATAL:
            return "FATAL";
        //case LogSeverity::NONE:
        //    return "NONE ";
        default:
            return NULL;
    }
}

/**
 * @brief translates developer string level to internal type
 * 
 * @param severity log severity level
 * @return internal type LogSeverity
 */
LogSeverity Unified_logger::StringToLevel(std::string severity)
{
    if (severity == "TRACE")
    {
        return LogSeverity::TRACE;
    }
    else if (severity == "DEBUG")
    {
        return LogSeverity::DEBUG;
    }
    else if (severity == "WARN")
    {
        return LogSeverity::WARN;
    }
    else if (severity == "INFO")    
    {
        return LogSeverity::INFO;  
    }
    else if (severity == "ERROR")
    {
        return LogSeverity::ERROR;
    }
    else if (severity == "FATAL")
    {
        return LogSeverity::FATAL;  
    }
    else
    {
        throw "Unexpected String representation of log level";
    }
        //case "NONE":
        //    return LogSeverity::NONE;       
}

/**
 * @brief Gets tag name for log message
 * @details Tag name is composed from machine name, logger id and message tag
 * 
 * @param tag message tag
 * @return tag name for event
 */
std::string Unified_logger::getTagHierarchy(std::string tag)
{
    char myhost[20];

    std::ostringstream id;
    gethostname(myhost, sizeof(myhost));
    id << "beeeon" << "." << myhost << "." << _logger_id << "." << tag;
    return id.str();
}


/**
 * @brief returns std::cout stream to handle message
 * @details constructs locked stream which handles mutex moving and implements << operator
 * 
 * @param location file origin of event returned by __FILE__
 * @param line line origin of event returned by __LINE__
 * @param tag event tag
 * @param level event severity level
 * @return locked stream object
 */
locked_stream Unified_logger::out(std::string location, int line, std::string tag, std::string level)
{
    if (StringToLevel(level) < _default_log_level)
    {
        return locked_stream(_filtered, getTagHierarchy(tag), level, location, line);
    }
    return locked_stream(std::cout, getTagHierarchy(tag), level, location, line);
}

/**
 * @brief returns file stream to handle message
 * @details constructs locked stream which handles mutex moving and implements << operator
 * 
 * @param location file origin of event returned by __FILE__
 * @param line line origin of event returned by __LINE__
 * @param tag event tag
 * @param level event severity level
 * @return locked stream object
 */
locked_stream Unified_logger::file(std::string location, int line, std::string tag, std::string level)
{
    if (StringToLevel(level) < _default_log_level)
    {
        return locked_stream(_filtered, getTagHierarchy(tag), level, location, line);
    }
    return locked_stream(_logfile, getTagHierarchy(tag), level, location, line);
}

