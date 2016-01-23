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

CalendarEvent::CalendarEvent(std::chrono::system_clock::time_point activation_time, std::string text):
    m_activation_time(activation_time), m_text(text)
{
    
    std::cout << "CALENDAR EVENT CONST:" << std::endl;
    std::cout << m_text << std::endl;
    std::cout << m_activation_time.time_since_epoch().count() << std::endl;
    std::cout << "----------" << std::endl;
    
}

CalendarEvent::CalendarEvent(const CalendarEvent& orig) {
}

CalendarEvent::~CalendarEvent() {
}

