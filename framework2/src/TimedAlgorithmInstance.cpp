/* 
 * File:   TimedAlgorithmInstance.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 22. January 2016
 */

#include "TimedAlgorithmInstance.h"

#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "Calendar.h"

TimedAlgorithmInstance::TimedAlgorithmInstance(unsigned long user_id, unsigned int users_instance_personal_id):
    AlgorithmInstance(user_id, users_instance_personal_id)
{   
    planActivationNow();
}

/*
TimedAlgorithmInstance::TimedAlgorithmInstance(const TimedAlgorithmInstance& orig) {
}

TimedAlgorithmInstance::~TimedAlgorithmInstance() {
}
*/

void TimedAlgorithmInstance::activate() {
    
    m_activation_mx.lock();
        
    //test.lock();
    run();
    
    //test.unlock();
    m_activation_mx.unlock();
}

void TimedAlgorithmInstance::run() {
    std::cout << "WRONG RUN" << std::endl;
}

void TimedAlgorithmInstance::planActivationNow() {
    Calendar::emplaceEvent(this);
}

void TimedAlgorithmInstance::planActivationAfterSeconds(int seconds) {
    Calendar::emplaceEvent(seconds, this);
}