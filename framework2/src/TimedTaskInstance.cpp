/* 
 * File:   TimedAlgorithmInstance.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 22. January 2016
 */

#include "TimedTaskInstance.h"

#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "Calendar.h"

TimedTaskInstance::TimedTaskInstance(unsigned int instance_id, TaskManager *owning_manager):
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
    std::cout << "TimedTaskInstance::activate() - enter" << std::endl;
    m_activation_mx.lock();
    
    
    //test.lock();
    run(activation_time);
    
    //test.unlock();
    m_activation_mx.unlock();
    std::cout << "TimedTaskInstance::activate() - leave" << std::endl;
}
/*
void TimedTaskInstance::run(std::chrono::system_clock::time_point activation_time)
{
    std::cout << "WRONG RUN" << std::endl;
}
*/
void TimedTaskInstance::planActivationNow()
{
    std::chrono::system_clock::time_point activation_time = Calendar::getInstance()->planActivation(this);
    m_activation_times.insert(activation_time);
    //Calendar::emplaceEvent(this);
    //Calendar::emplaceEvent(this->shared_from_this());
}

void TimedTaskInstance::planActivationAfterSeconds(int seconds)
{
    std::chrono::system_clock::time_point activation_time = Calendar::getInstance()->planActivation(seconds, this);
    m_activation_times.insert(activation_time);
    //Calendar::emplaceEvent(seconds, this);
    //Calendar::emplaceEvent(seconds, this->shared_from_this());
}

void TimedTaskInstance::removeFromCalendar()
{
    Calendar::getInstance()->removeAllActivationsOfInstance(m_activation_times, this);
}
