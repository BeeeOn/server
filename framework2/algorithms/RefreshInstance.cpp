/* 
 * File:   RefreshInstance.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 21. January 2016
 */

#include "RefreshInstance.h"

#include <iostream>
#include <string>

#include "../src/Calendar.h"

RefreshInstance::RefreshInstance(unsigned int manager_id, unsigned long instance_id, unsigned long user_id, unsigned int users_instance_personal_id, std::string text):
    TimedAlgorithmInstance(manager_id, instance_id, user_id, users_instance_personal_id, text)
{
    // Activate after 5 seconds.
    planActivationAfterSeconds(4);
}
/*
RefreshInstance::RefreshInstance(const RefreshInstance& orig) {
}
*/
RefreshInstance::~RefreshInstance() {
}


void RefreshInstance::activate() {
   
    std::cout << "REFRESH INSTANCE ACTIVATED: " <<  m_instance_id << " | " << m_activated_times++ << " TIMES : " << m_text << std::endl;
    
    planActivationAfterSeconds(4);
}
/*
void RefreshInstance::planActivationAfterSeconds(int seconds) {
    std::cout << "EMPLACING EVENT REFRESH." << std::endl;
    //Calendar::emplaceEvent(seconds, m_manager_id, m_instance_id);
    Calendar::emplaceEvent(seconds, getptr());
    std::cout << "EMPLACING EVENT REFRESH2." << std::endl;
}*/