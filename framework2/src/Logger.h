/* 
 * File:   Logger.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 20. January 2016
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <mutex>
#include <string>
#include <ctime>

class Logger {
public:
    Logger();
    Logger(const Logger& orig);
    virtual ~Logger();
    
    static std::mutex logger_mutex;
    
    static void logToCout(std::string log);

    static void logToCerr(std::string log);
    
private:
    static time_t getCurrentTime();
};

#endif /* LOGGER_H */

