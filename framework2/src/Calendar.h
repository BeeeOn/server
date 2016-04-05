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
#include <map>
#include <memory> //std::weak_ptr
#include <mutex> //std::mutex
#include <vector>

#include "TaskInstance.h"

//class TaskInstance;
class Calendar {
public:
    /**
     * Create instance of Calendar singleton, or return it.
     * @return Pointer to Calendar.
     */
    static std::shared_ptr<Calendar> getInstance();
    
    /**
     * Destructor of class Calendar.
     */
    virtual ~Calendar();
    
    /*
     * Main algorithm of calendar. Must run only once in a seperated thread.
     */
    void runCalendar();
    
    /**
     * Calculates activation time: now + seconds and planes instance activation to calendar with calculated activation time.
     * @param seconds Relative activation time (activate seconds from now).
     * @param instance_ptr Pointer to instance, which should be activated.
     */
    void planActivation(int seconds, TaskInstance* instance_ptr);
    
    /**
     * Immediately plans instance activation to calendar with current time.
     * @param instance_ptr Pointer to instance, which should be activated.
     */
    void planActivation(TaskInstance* instance_ptr);
  
    /**
     * Stops calendar and saves activation times.
     */
    void stopCalendar();

    /**
     * Deletes all planned activations from calendar of passed TaskInstance.
     * @param instance_ptr Pointer to instance which should be deleted from calendar.
     */
    void removeAllActivations(TaskInstance* instance_ptr);
    
private:
    /**
     * Constructor of class Calendar.
     */
    Calendar();
    /**
     * Delete copy constructor.
     */
    Calendar(const Calendar& orig) = delete;
    /**
     * Delete assignment operator.
     */
    void operator=(const Calendar&) = delete;
    /**
     * Pointer to Calendar instance. 
     */
    static std::shared_ptr<Calendar> m_instance;   
 
    /*
     * Activates instances passed in parameter.
     * @param to_activate Instances which should be activated.
     */
    void activateInstances(std::vector<TaskInstance*> to_activate);
    
    /**
     * If calendar is empty causes wait until it is notified that new event was emplaced to queue.
     */
    void waitUntilCalendarIsNotEmpty();
 
    /**
     * Emplaces event to multimap.
     * @param activation_time Time at which should event activate.
     * @param instance_ptr Pointer to instance, which should be activated.
     */
    void emplaceEvent(std::chrono::system_clock::time_point activation_time, TaskInstance* instance_ptr);
    
    // Time until main algorithm should wait.
    std::chrono::system_clock::time_point m_wakeup_time;
    
    // Priority queue holding calendar events, those ones with smallest activation time are at top.
    //std::priority_queue<std::shared_ptr<CalendarEvent>, std::vector<std::shared_ptr<CalendarEvent>>, GreaterCalendarEventSharedPtr> m_calendar_events;
    std::multimap<std::chrono::system_clock::time_point, TaskInstance*> m_calendar_events; 
    
    /**
     * Contition variables for waking up main algorithm.
     */
    std::condition_variable m_new_wakeup_time_cv;
    std::mutex m_new_wakeup_time_mx;
    std::condition_variable m_queue_not_empty_cv;
    std::mutex m_queue_not_empty_mx;
    
    std::mutex m_calendar_events_mx;
    std::mutex m_test_queue_empty_mx;

    // Indicates if calendar algorithm should run.
    bool m_should_run;
    
    // Indicates if Calendar runns, so it can't be run again.
    bool m_running;
};
#endif /* CALENDAR_H */
