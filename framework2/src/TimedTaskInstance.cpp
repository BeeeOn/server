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

TimedTaskInstance::TimedTaskInstance(unsigned int instance_id):
    TaskInstance(instance_id)
{
}

/*
TimedAlgorithmInstance::TimedAlgorithmInstance(const TimedAlgorithmInstance& orig) {
}
*/
TimedTaskInstance::~TimedTaskInstance()
{
}


void TimedTaskInstance::activate()
{
    std::cout << "TimedTaskInstance::activate() - enter" << std::endl;
    m_activation_mx.lock();
    
    //test.lock();
    run();
    
    //test.unlock();
    m_activation_mx.unlock();
    std::cout << "TimedTaskInstance::activate() - leave" << std::endl;
}

void TimedTaskInstance::run()
{
    std::cout << "WRONG RUN" << std::endl;
}

void TimedTaskInstance::planActivationNow()
{
    Calendar::getInstance()->planActivation(this);
    //Calendar::emplaceEvent(this);
    //Calendar::emplaceEvent(this->shared_from_this());
}

void TimedTaskInstance::planActivationAfterSeconds(int seconds)
{
    Calendar::getInstance()->planActivation(seconds, this);
    //Calendar::emplaceEvent(seconds, this);
    //Calendar::emplaceEvent(seconds, this->shared_from_this());
}

void TimedTaskInstance::deleteInstance()
{
    Calendar::getInstance()->removeAllActivations(this);
}
