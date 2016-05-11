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

TimedTaskInstance::TimedTaskInstance(long instance_id, std::weak_ptr<TaskManager> owning_manager):
    TaskInstance(instance_id, owning_manager)
{
}

TimedTaskInstance::~TimedTaskInstance()
{
}

void TimedTaskInstance::activate(std::chrono::system_clock::time_point activation_time)
{
    // Run instance, but protect it with lock_guard
    // so two runs at the same time are not possible.
    std::lock_guard<std::mutex> lock(m_activation_mx); 
    
    try {
        run(activation_time);
        // Remove recently activated time.
        m_activation_times.erase(activation_time);
    }
    catch (const std::exception& e) {
        logger.LOGFILE("timed_instance", "ERROR") << e.what() << std::endl;
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

void TimedTaskInstance::planActivationToDateAndTime(std::string date_time)
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

void TimedTaskInstance::deleteFromControlComponent()
{
    try {
        // Remove all activations of instance from Calendar.
        Calendar::getInstance()->removeAllActivationsOfInstance(m_activation_times, this);
    }
    catch (const std::exception& e) {
        logger.LOGFILE("timed_instance", "WARN") << e.what() << std::endl;
    }
}

void TimedTaskInstance::removePlannedActivation(std::chrono::system_clock::time_point activation_time)
{
    try {
        // Remove activation of instance from Calendar.
        Calendar::getInstance()->removeActivation(activation_time, this);
    }
    catch (const std::exception& e) {
        logger.LOGFILE("timed_instance", "WARN") << e.what() << std::endl;
    }
}
