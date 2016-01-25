/* 
 * File:   CalendarEvent.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 20. January 2016
 */

#include "CalendarEvent.h"

#include <iostream>
#include <string>

#include "Logger.h"
/*
CalendarEvent::CalendarEvent(std::chrono::system_clock::time_point activation_time, unsigned int manager_id, unsigned long instance_id):
    m_activation_time(activation_time), m_manager_id(manager_id), m_instance_id(instance_id)
{
}
*/
//CalendarEvent::CalendarEvent(std::chrono::system_clock::time_point activation_time, std::weak_ptr<TimedAlgorithmInstance> instance_ptr):
CalendarEvent::CalendarEvent(std::chrono::system_clock::time_point activation_time, TimedAlgorithmInstance *instance_ptr):
    m_activation_time(activation_time), m_instance_ptr(instance_ptr)
{
    std::cout << "EVENT CONSTRUCT: " << m_instance_ptr->getText() << std::endl;
}
/*
CalendarEvent::CalendarEvent(const CalendarEvent& orig) {
}
*/
CalendarEvent::~CalendarEvent() {
}