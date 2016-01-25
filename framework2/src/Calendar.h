/* 
 * File:   Calendar.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 20. January 2016
 */

#ifndef CALENDAR_H
#define CALENDAR_H

//#include <map> //std::multimap

#include <atomic>
#include <condition_variable>
#include <chrono>
#include <vector>
#include <memory>
#include <mutex>
#include <queue> //std::priority_queue
#include <string>

#include "CalendarEvent.h"
#include "TimedAlgorithmInstance.h"

// Class important for correct ordering of CalendarEvents in m_calendar_events priority_queue (smallest activation time first)
class GreaterCalendarEventSharedPtr {
public:
    bool operator()(const std::shared_ptr<CalendarEvent> lhs, const std::shared_ptr<CalendarEvent> rhs)
    {
        return lhs->getActivationTime() > rhs->getActivationTime();
    }
};

class Calendar {
public:
    Calendar();
    
    Calendar(const Calendar& orig);
    
    virtual ~Calendar();
    
    void run();
    
    void executeEvents(std::vector<std::shared_ptr<CalendarEvent>> events_to_execute);
    
    long calculateSleepTimeMs();
    
    void waitUntilCalendarIsNotEmpty();
    
    
    static std::condition_variable cv, empty_queue_cv;
    
    static std::mutex cv_m, empty_queue_m;
 
    static std::mutex m_calendar_events_mutex;
    
    // Emplaces event into multimap of events.
    //static void emplaceEvent(int seconds, std::string text);
    //static void emplaceEvent(int seconds, unsigned int manager_id, unsigned long instance_id);
    //static void emplaceEvent(int wait_time, std::weak_ptr<TimedAlgorithmInstance> instance_ptr);
    static void emplaceEvent(int wait_time, TimedAlgorithmInstance *instance_ptr);
    
    // Time when thread which activates events should run.
    static std::chrono::system_clock::time_point m_wakeup_time;
    
    // Priority queue holding calendar events, those ones with smallest activation time are at top.
    //static std::priority_queue<std::shared_ptr<CalendarEvent>, std::vector<std::shared_ptr<CalendarEvent>>, GreaterCalendarEventSharedPtr> m_calendar_events;
    static std::priority_queue<std::shared_ptr<CalendarEvent>, std::vector<std::shared_ptr<CalendarEvent>>, GreaterCalendarEventSharedPtr> m_calendar_events;
    //static std::priority_queue<std::pair<std::chrono::system_clock::time_point /*activation_time*/, std::weak_ptr<TimedAlgorithmInstance> /*instance_ptr*/>,
      //                         std::vector<std::pair<std::chrono::system_clock::time_point /*activation_time*/, std::weak_ptr<TimedAlgorithmInstance> /*instance_ptr*/>,
        //                       GreaterCalendarEvent> m_calendar_events;
    
    // Getters.
    std::chrono::system_clock::time_point getWakeupTime() { return m_wakeup_time; }

private:
    
    int m_count_events_to_execute;
};

#endif /* CALENDAR_H */

