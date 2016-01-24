/* 
 * File:   TimedAlgorithmInstance.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 22. January 2016
 */

#include "TimedAlgorithmInstance.h"

#include <iostream>
#include <string>

#include "Calendar.h"

TimedAlgorithmInstance::TimedAlgorithmInstance(unsigned int manager_id, unsigned long instance_id, unsigned long user_id, unsigned int users_instance_personal_id, std::string text):
    AlgorithmInstance(manager_id, instance_id, user_id, users_instance_personal_id, text)
{
    // Activate after 5 seconds.
    planActivationForSeconds(4);
    
}

/*
TimedAlgorithmInstance::TimedAlgorithmInstance(const TimedAlgorithmInstance& orig) {
}

TimedAlgorithmInstance::~TimedAlgorithmInstance() {
}
*/
void TimedAlgorithmInstance::activate() {
    std::cout << "TIMED INSTANCE ACTIVATED: " <<  m_instance_id << ": " << m_text << std::endl;
    planActivationForSeconds(4);
}

void TimedAlgorithmInstance::planActivationForSeconds(int seconds) {
    
    Calendar::emplaceEvent(seconds, m_manager_id, m_instance_id);
}