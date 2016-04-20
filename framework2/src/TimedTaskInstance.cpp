/* 
 * File:   TimedAlgorithmInstance.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 22. January 2016
 */

#include "TimedTaskInstance.h"

#include <chrono>
#include <mutex>

#include "Calendar.h"
#include "Logger.h"

TimedTaskInstance::TimedTaskInstance(int instance_id, std::weak_ptr<TaskManager> owning_manager):
    TaskInstance(instance_id, owning_manager)
{
}

TimedTaskInstance::~TimedTaskInstance()
{
    // Before destruction, remove the instance from calendar.
    removeFromCalendar();
}

void TimedTaskInstance::activate(std::chrono::system_clock::time_point activation_time)
{
    // Run instance, but protect it with lock_guard
    // so two runs at the same time are not possible.
    std::lock_guard<std::mutex> lock(m_activation_mx); 
    
    try {
        run(activation_time);
    }
    catch (const std::exception& e) {
        logger.LOGFILE("trigger_instance", "ERROR") << e.what() << std::endl;
    }
}

void TimedTaskInstance::planActivationNow()
{
    try {
        std::chrono::system_clock::time_point activation_time = Calendar::getInstance()->planActivation(this);
        m_activation_times.insert(activation_time);
    }
    catch (const std::exception& e) {
        logger.LOGFILE("timed_instance", "WARN") << e.what() << std::endl;
    }
}

void TimedTaskInstance::planActivationAfterSeconds(int seconds)
{
    try {
        std::chrono::system_clock::time_point activation_time = Calendar::getInstance()->planActivation(seconds, this);
        m_activation_times.insert(activation_time);
    }
    catch (const std::exception& e) {
        logger.LOGFILE("timed_instance", "WARN") << e.what() << std::endl;
    }
}

void TimedTaskInstance::planToDateAndTime(std::string date_time)
{
    try {
        std::chrono::system_clock::time_point activation_time = Calendar::getInstance()->planActivation(date_time, this);
        m_activation_times.insert(activation_time);
    }
    catch (const std::exception& e) {
        logger.LOGFILE("timed_instance", "WARN") << e.what() << std::endl;
    }
    std::tm tm = {};
}

void TimedTaskInstance::removeFromCalendar()
{
    try {
        Calendar::getInstance()->removeAllActivationsOfInstance(m_activation_times, this);
    }
    catch (const std::exception& e) {
        logger.LOGFILE("timed_instance", "WARN") << e.what() << std::endl;
    }
}
