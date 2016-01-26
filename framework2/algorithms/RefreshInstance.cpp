/* 
 * File:   RefreshInstance.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 21. January 2016
 */

#include "RefreshInstance.h"

#include <iostream>
#include <string>
#include <chrono>
#include <thread>

RefreshInstance::RefreshInstance(unsigned long user_id, unsigned int users_instance_personal_id):
    TimedAlgorithmInstance(user_id, users_instance_personal_id)
{
    // Activate now.
    
}

RefreshInstance::~RefreshInstance() {
}

void RefreshInstance::run() {
    refreshCustomFunction();
}

void RefreshInstance::refreshCustomFunction() {
    // Activate after 5 seconds.
    planActivationAfterSeconds(5);
    
    std::cout << "REFRESH INSTANCE ACTIVATED: " << m_activated_times++ << " TIMES" << std::endl << m_text_to_write << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "REFRESH INSTANCE FINISHED" << std::endl << "-----------" << std::endl;
}
