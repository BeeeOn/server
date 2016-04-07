#include "unified_logger.h"

Unified_logger::Unified_logger(std::string log_folder_path, std::string logger_id, int default_log_level):
    _log_folder_path(log_folder_path),
    _logger_id(logger_id),
    _default_log_level(default_log_level)
{
}

//Default values
Unified_logger::Unified_logger(std::string logger_id):
    _log_folder_path("."),
    _logger_id(logger_id),
    _default_log_level(ERROR)
{
    _logfile.open( getLogFilePath(), std::ios::app);
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
 * @brief returns stream to handle message
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
    return locked_stream(std::cout, getTagHierarchy(tag), level, location, line);
}

locked_stream Unified_logger::file(std::string location, int line, std::string tag, std::string level)
{
    return locked_stream(_logfile, getTagHierarchy(tag), level, location, line);
}

