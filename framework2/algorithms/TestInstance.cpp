/* 
 * File:   TestInstance.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 26. January 2016
 */

#include "TestInstance.h"

#include <iostream>
#include <string>
#include <chrono>
#include <thread>

#include "../src/Calendar.h"

TestInstance::TestInstance(unsigned long user_id, unsigned int users_instance_personal_id):
    TimedTaskInstance(user_id, users_instance_personal_id)
{
    // Activate now.
    
}

TestInstance::~TestInstance() {
}

void TestInstance::run() {
    planActivationAfterSeconds(2);
    std::cout << "TEST INSTANCE ACTIVATED: " << m_activated_times++ << " TIMES" << std::endl << m_text_to_write << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "TEST INSTANCE FINISHED" << std::endl << "-----------" << std::endl;
}