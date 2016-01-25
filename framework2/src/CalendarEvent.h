/* 
 * File:   CalendarEvent.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 20. January 2016
 */

#ifndef CALENDAREVENT_H
#define CALENDAREVENT_H

#include <chrono>
#include <memory>
#include <string>

#include "TimedAlgorithmInstance.h"

class CalendarEvent {
public:
    // Test purpose constructor.
    //CalendarEvent(std::chrono::system_clock::time_point activation_time, std::string text);
    
    //CalendarEvent(std::chrono::system_clock::time_point activation_time, unsigned int manager_id, unsigned long instance_id);
    //CalendarEvent(std::chrono::system_clock::time_point activation_time, std::weak_ptr<TimedAlgorithmInstance> instance_ptr);
    CalendarEvent(std::chrono::system_clock::time_point activation_time, TimedAlgorithmInstance *instance_ptr);
    //CalendarEvent(const CalendarEvent& orig);
    
    virtual ~CalendarEvent();
    
    // Getters.
    std::chrono::system_clock::time_point getActivationTime() const {return m_activation_time;};
   
    //std::string getText() const {return m_text;};
    
    // Determine priority.
  
    friend bool operator <(const CalendarEvent& lhs, const CalendarEvent& rhs)
    {    
        return lhs.getActivationTime() < rhs.getActivationTime();
    }

    friend bool operator >(const CalendarEvent& lhs, const CalendarEvent& rhs)
    {    
        return lhs.getActivationTime() > rhs.getActivationTime();
    }
    
    //unsigned long m_instance_id;
    
    //unsigned int m_manager_id;
    
    //std::weak_ptr<TimedAlgorithmInstance> m_instance_ptr;
    TimedAlgorithmInstance *m_instance_ptr;
private:
    
    std::chrono::system_clock::time_point m_activation_time;
    
    
    
    //std::string m_text; 
    
    
};

#endif /* CALENDAREVENT_H */

