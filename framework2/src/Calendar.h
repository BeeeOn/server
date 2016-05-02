/* 
 * File:   Calendar.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 20. January 2016
 */

#ifndef CALENDAR_H
#define CALENDAR_H

#include <condition_variable>
#include <chrono>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <vector>

#include "TaskInstance.h"

class Calendar {
public:
    /**
     * Creates singleton instance. Must be called just once in entire program.
     */
    static void createInstance();    
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
    std::chrono::system_clock::time_point planActivation(int seconds, TaskInstance* instance_ptr);
    /**
     * Immediately plans instance activation to calendar with current time.
     * @param instance_ptr Pointer to instance, which should be activated.
     */
    std::chrono::system_clock::time_point planActivation(TaskInstance* instance_ptr);
    /**
     * Plans activation of instance on specific date and time.
     * It must have format: "1 9 2014 12:35:34" -> "month day_of_month year time"
     * Must be UTC.
     * @param date_time Formatted string with date and time to activate.
     * @param instance_ptr Poiter to instance which should be activated.
     * @return Time of activation.
     */
    std::chrono::system_clock::time_point planActivation(std::string date_time, TaskInstance* instance_ptr);
    /**
     * Stops calendar and saves activation times.
     */
    void stopCalendar();
    /**
     * Deletes all planned activations from calendar of passed TaskInstance.
     * @param instance_ptr Pointer to instance which should be deleted from calendar.
     */
    void removeAllActivationsOfInstance(std::set<std::chrono::system_clock::time_point> planned_times, TaskInstance* instance_ptr);
    /**
     * Removes one activation of instnace from Calendar.
     * @param activation_time Time of activation.
     * @param instance_ptr Pointer to instance which activation should be removed.
     */
    void removeActivation(std::chrono::system_clock::time_point activation_time, TaskInstance* instance_ptr);
    
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
    void activateInstances(std::multimap<std::chrono::system_clock::time_point, TaskInstance*> to_activate);
    /**
     * If calendar is empty causes wait until it is notified that new event was emplaced to queue.
     */
    void waitUntilCalendarIsNotEmpty();
    /**
     * Emplaces event to multimap.
     * @param activation_time Time at which should event activate.
     * @param instance_ptr Pointer to instance, which should be activated.
     */
    void emplaceActivation(std::chrono::system_clock::time_point activation_time, TaskInstance* instance_ptr);
    /**
     *  Time until main algorithm should wait.
     */
    std::chrono::system_clock::time_point m_wakeup_time;
    /**
     *  Multimap holding calendar events, those ones with smallest activation time are at the beginning.
     */
    std::multimap<std::chrono::system_clock::time_point, TaskInstance*> m_calendar_events;
    /**
     * Contition variables for waking up main algorithm.
     */
    std::condition_variable m_new_wakeup_time_cv;
    std::mutex m_new_wakeup_time_mx;
    std::condition_variable m_calendar_not_empty_cv;
    std::mutex m_calendar_not_empty_mx;
    
    std::mutex m_calendar_events_mx;
    std::mutex m_test_calendar_empty_mx;
    /**
     *  Indicates if calendar algorithm should run.
     */
    bool m_should_run;
    /**
     * Indicates if Calendar runns, so it can't be run again.
     */
    bool m_running;
};

#endif /* CALENDAR_H */
