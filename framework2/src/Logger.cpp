/* 
 * File:   Logger.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 20. January 2016
 */

#include "Logger.h"

#include <ctime>
#include <iostream>
#include <chrono>
#include <string>

// Definitions of static variables.
std::mutex Logger::logger_mutex;

Logger::Logger() {
}

Logger::Logger(const Logger& orig) {
}

Logger::~Logger() {
}

void Logger::logToCout(std::string log) {
    logger_mutex.lock();
    std::cout << log << std::endl;
    logger_mutex.unlock();
}

void Logger::logToCerr(std::string log) {
    time_t tn = getCurrentTime();
    std::cerr << ctime(&tn) << ": " << log << std::endl;
}

time_t Logger::getCurrentTime() {
    // Get current time.
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    // Return converted to time_t.
    return std::chrono::system_clock::to_time_t(now);
}
