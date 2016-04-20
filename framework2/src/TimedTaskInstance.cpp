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
    logger.LOGFILE("timed_instance", "TRACE") << "TimedTaskInstance::~TimedTaskInstance - enter" << std::endl;
    // Before destruction, remove the instance from calendar.
    removeFromCalendar();
    
    logger.LOGFILE("timed_instance", "TRACE") << "TimedTaskInstance::~TimedTaskInstance - leave" << std::endl;
}

void TimedTaskInstance::activate(std::chrono::system_clock::time_point activation_time)
{
    logger.LOGFILE("timed_instance", "TRACE") << "TimedTaskInstance::activate() - enter" << std::endl;
    
    // Run instance, but protect it with lock_guard
    // so two runs at the same time are not possible.
    std::lock_guard<std::mutex> lock(m_activation_mx); 
    
    try {
        run(activation_time);
    }
    catch (const std::exception& e) {
        logger.LOGFILE("trigger_instance", "ERROR") << e.what() << std::endl;
    }
    
    logger.LOGFILE("timed_instance", "TRACE") << "TimedTaskInstance::activate() - leave" << std::endl;
}

void TimedTaskInstance::planActivationNow()
{
    logger.LOGFILE("timed_instance", "TRACE") << "TimedTaskInstance::planActivationNow - enter" << std::endl;
    
    try {
        std::chrono::system_clock::time_point activation_time = Calendar::getInstance()->planActivation(this);
        m_activation_times.insert(activation_time);
    }
    catch (const std::exception& e) {
        logger.LOGFILE("timed_instance", "WARN") << e.what() << std::endl;
    }
    logger.LOGFILE("timed_instance", "TRACE") << "TimedTaskInstance::planActivationNow - leave" << std::endl;
}

void TimedTaskInstance::planActivationAfterSeconds(int seconds)
{
    logger.LOGFILE("timed_instance", "TRACE") << "TimedTaskInstance::planActivationAfterSeconds - enter" << std::endl;
    
    try {
        std::chrono::system_clock::time_point activation_time = Calendar::getInstance()->planActivation(seconds, this);
        m_activation_times.insert(activation_time);
    }
    catch (const std::exception& e) {
        logger.LOGFILE("timed_instance", "WARN") << e.what() << std::endl;
    }
    logger.LOGFILE("timed_instance", "TRACE") << "TimedTaskInstance::planActivationAfterSeconds - leave" << std::endl;
}

void TimedTaskInstance::removeFromCalendar()
{
    logger.LOGFILE("timed_instance", "TRACE") << "TimedTaskInstance::removeFromCalendar - enter" << std::endl;
    
    try {
        Calendar::getInstance()->removeAllActivationsOfInstance(m_activation_times, this);
    }
    catch (const std::exception& e) {
        logger.LOGFILE("timed_instance", "WARN") << e.what() << std::endl;
    }
    logger.LOGFILE("timed_instance", "TRACE") << "TimedTaskInstance::removeFromCalendar - leave" << std::endl;
}
