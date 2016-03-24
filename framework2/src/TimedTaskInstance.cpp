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

TimedTaskInstance::TimedTaskInstance() 
{   
    
}

/*
TimedAlgorithmInstance::TimedAlgorithmInstance(const TimedAlgorithmInstance& orig) {
}
*/
TimedTaskInstance::~TimedTaskInstance() {
}


void TimedTaskInstance::activate() {
    
    m_activation_mx.lock();
        
    //test.lock();
    run();
    
    //test.unlock();
    m_activation_mx.unlock();
}

void TimedTaskInstance::run() {
    std::cout << "WRONG RUN" << std::endl;
}

void TimedTaskInstance::planActivationNow() {
    Calendar::emplaceEvent(this);
}

void TimedTaskInstance::planActivationAfterSeconds(int seconds) {
    Calendar::emplaceEvent(seconds, this);
}