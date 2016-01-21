/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Logger.h
 * Author: mrmaidx
 *
 * Created on 20. ledna 2016, 14:15
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <ctime>

class Logger {
public:
    Logger();
    Logger(const Logger& orig);
    virtual ~Logger();
    
    static void logToCout(std::string log);

    static void logToCerr(std::string log);
    
private:
    static time_t getCurrentTime();
};

#endif /* LOGGER_H */

