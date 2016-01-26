/* 
 * File:   Calendar.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 20. January 2016
 */

#ifndef CALENDAR_H
#define CALENDAR_H

#include <condition_variable> //std::condition_variable
#include <chrono> //std::time_point
#include <vector>
#include <memory> //std::weak_ptr
#include <mutex> //std::mutex
#include <queue> //std::priority_queue

#include "CalendarEvent.h"
#include "TimedAlgorithmInstance.h"

// Class important for correct ordering of CalendarEvents in m_calendar_events priority_queue (event with lowest activation time at top).
class GreaterCalendarEventSharedPtr {
public:
    bool operator()(const std::shared_ptr<CalendarEvent> lhs, const std::shared_ptr<CalendarEvent> rhs)
    {
        return lhs->getActivationTime() > rhs->getActivationTime();
    }
};

class Calendar {
public:
    /**
     * Constructor of class Calendar.
     */
    Calendar();
    
    /**
     * Destructor of class Calendar.
     */
    virtual ~Calendar();
    
    /*
     * Main algorithm of calendar. Should run only once in a seperated thread.
     */
    void run();
    
    /*
     * Executes events passed in parameter (activates instances stored in passed events).
     * @param events_to_execute Events which should be executed.
     */
    void executeEvents(std::vector<std::shared_ptr<CalendarEvent>> events_to_execute);
    
    /**
     * If queue is empty causes wait until it is notified that new event was emplaced to queue.
     */
    void waitUntilCalendarIsNotEmpty();
    
    /**
     * Contition variables for waking up main algorithm.
     */
    static std::condition_variable m_new_wakeup_time_cv;
    static std::condition_variable m_queue_not_empty_cv;
    
    static std::mutex m_calendar_events_mx;
    static std::mutex m_new_wakeup_time_mx;
    static std::mutex m_queue_not_empty_mx;
    static std::mutex m_mx;
    
    //static void emplaceEvent(int wait_time, std::weak_ptr<TimedAlgorithmInstance> instance_ptr);
    /**
     * Calculates activation time: now + seconds and pushes event to queue with calculated activation time.
     * @param seconds Relative activation time (activate seconds from now).
     * @param instance_ptr Pointer to instance, which should be activated.
     */
    static void emplaceEvent(int seconds, TimedAlgorithmInstance *instance_ptr);
    
    /**
     * Immediately pushes event to queue with current time.
     * @param instance_ptr Pointer to instance, which should be activated.
     */
    static void emplaceEvent(TimedAlgorithmInstance *instance_ptr);
    
    /**
     * Pushes event to queue.
     * @param activation_time Time at which should event activate.
     * @param instance_ptr Pointer to instance, which should be activated.
     */
    static void pushEvent(std::chrono::system_clock::time_point activation_time, TimedAlgorithmInstance *instance_ptr);
    
    // Time until main algorithm should wait.
    static std::chrono::system_clock::time_point m_wakeup_time;
    
    // Priority queue holding calendar events, those ones with smallest activation time are at top.
    static std::priority_queue<std::shared_ptr<CalendarEvent>, std::vector<std::shared_ptr<CalendarEvent>>, GreaterCalendarEventSharedPtr> m_calendar_events;
};

#endif /* CALENDAR_H */

