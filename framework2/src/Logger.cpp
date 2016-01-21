/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Logger.cpp
 * Author: mrmaidx
 * 
 * Created on 20. ledna 2016, 14:15
 */

#include "Logger.h"

#include <iostream>
#include <chrono>
#include <ctime>

Logger::Logger() {
}

Logger::Logger(const Logger& orig) {
}

Logger::~Logger() {
}

void Logger::logToCout(std::string log) {
    time_t tn = getCurrentTime();
    std::cout << ctime(&tn) << ": " << log << std::endl;
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
