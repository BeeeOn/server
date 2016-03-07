/* 
 * File:   CalendarEvent.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 20. January 2016
 */

#include "CalendarEvent.h"

//#include <iostream>
//#include <memory>

//CalendarEvent::CalendarEvent(std::chrono::system_clock::time_point activation_time, std::weak_ptr<TimedAlgorithmInstance> instance_ptr):
CalendarEvent::CalendarEvent(std::chrono::system_clock::time_point activation_time, TimedTaskInstance *instance_ptr):
    m_activation_time(activation_time), m_instance_ptr(instance_ptr)
{
}

CalendarEvent::~CalendarEvent() {
}

void CalendarEvent::activateInstance() {
    // Activates instance (WARNING! Does not check if pointed instance still exists!).
    m_instance_ptr->activate();

    /*
    // Activation when m_instance_ptr is std::weak_ptr. Checks instance existence.
    if (auto locked_instance_ptr = m_instance_ptr.lock()) {
        locked_instance_ptr->activate();
    }
    else {
        std::cerr << "Instance was deleted before it could be activated." << std::endl;
    }
    */
}
