/* 
 * File:   TimedTaskInstance.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 22. January 2016
 */

#ifndef TIMEDTASKINSTANCE_H
#define TIMEDTASKINSTANCE_H

#include <chrono>
#include <memory>
#include <mutex>
#include <set>
#include <string>

#include "TaskInstance.h"

class TimedTaskInstance: public TaskInstance
{
public:
    /**
     * Constrictor of class TimedTaskInstance.
     * @param instance_id ID of instance.
     * @param owning_manager Pointer to manager of Task of this instance.
     */
    TimedTaskInstance(int instance_id, std::weak_ptr<TaskManager> owning_manager);
    /**
     * Destructor of class TimedTaskInstance.
     */
    virtual ~TimedTaskInstance();
    /**
     * Plans to calendar when this instance shoud be activated again.
     * @param seconds Seconds after which activate again.
     */
    void planActivationAfterSeconds(int seconds);
    /*
     * Plans to calendar activation of and instance to present time.
     */
    void planActivationNow();
    /**
     * Plans instance activation to calendar to exact date and time.
     * It must have format: "1 9 2014 12:35:34"
     * "month day_of_month year time"
     */
    void planToDateAndTime(std::string date_time);  
    /**
     * Activates an instance (calls run() function, but protects it with mutex).
     * @param activation_time Time at which was instance activated.
     */
    void activate(std::chrono::system_clock::time_point activation_time) override;
    /**
     * Removes all timed entries of instance from calendar.
     */
    void removeFromCalendar();
    /**
     * Function which contains all functionality.
     */
    virtual void run(std::chrono::system_clock::time_point activation_time) = 0;
    
private:
    /**
     * Times at which was instance planned to calendar.
     */
    std::set<std::chrono::system_clock::time_point> m_activation_times;
    /**
     * Mutex which protects instance from being activated more than once.
     */
    std::mutex m_activation_mx;
};

#endif /* TIMEDTASKMINSTANCE_H */
