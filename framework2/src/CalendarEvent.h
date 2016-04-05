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

#include "TaskInstance.h"

class CalendarEvent {
public:
    /**
     * Constructor of class CalendarEvent.
     * @param activation_time Time at which should pointed instance activate.
     * @param instance_ptr Pointer to instance which should be activated.
     */
    CalendarEvent(std::chrono::system_clock::time_point activation_time, TaskInstance* instance_ptr);

    /**
     * Destructor of class CalendarEvent.
     */
    virtual ~CalendarEvent();
    
    /**
     * Dereferences instance pointer stored in m_instance_ptr and calls it's activate() function.
     */
    void activateInstance();
    
    /**
     * Getter of member variable m_activation_time.
     * @return Time at which should pointed instance activate.
     */
    std::chrono::system_clock::time_point getActivationTime() const {return m_activation_time;};
   
    /*
     * Overloaded operators for comparison of CalendarEvent class by m_activation_time.
     */
    friend bool operator <(const CalendarEvent& lhs, const CalendarEvent& rhs)
    {    
        return lhs.getActivationTime() < rhs.getActivationTime();
    }
    
    friend bool operator >(const CalendarEvent& lhs, const CalendarEvent& rhs)
    {    
        return lhs.getActivationTime() > rhs.getActivationTime();
    }

private:
    /**
     * Time at which should pointed instance activate.
     */
    std::chrono::system_clock::time_point m_activation_time;
    /**
     * Pointer to instance which should be activated.
     */
    TaskInstance* m_instance_ptr;
    //std::weak_ptr<TimedAlgorithmInstance> m_instance_ptr;
};
#endif /* CALENDAREVENT_H */