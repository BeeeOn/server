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

TimedAlgorithmInstance::TimedAlgorithmInstance(unsigned int manager_id, unsigned long instance_id, unsigned long user_id, unsigned int users_instance_personal_id, std::string text):
    AlgorithmInstance(manager_id, instance_id, user_id, users_instance_personal_id, text)
{   
    
}

/*
TimedAlgorithmInstance::TimedAlgorithmInstance(const TimedAlgorithmInstance& orig) {
}

TimedAlgorithmInstance::~TimedAlgorithmInstance() {
}
*/
void TimedAlgorithmInstance::activate() {
    //std::cout << "TIMED INSTANCE ACTIVATED: " <<  m_instance_id << ": " << m_text << std::endl;
    std::cout << "BASE TIMEDALGORITHMINSTANCE CLASS WAS ACTIVATED!" << std::endl;
}

void TimedAlgorithmInstance::planActivationAfterSeconds(int seconds) {
    //std::cout << "EMPLACING EVENT." << std::endl;
    //Calendar::emplaceEvent(seconds, m_manager_id, m_instance_id);
    //std::this_thread::sleep_for(std::chrono::seconds(10));
    //std::cout <<  shared_from_this()->getText() << std::endl;
    Calendar::emplaceEvent(seconds, this);
    //std::cout << "EMPLACING EVENT2." << std::endl;
}