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
#include <vector>

#include "TaskInstance.h"


class TimedTaskInstance: public TaskInstance//, std::enable_shared_from_this<TimedTaskInstance>
{
public:

    
    TimedTaskInstance(int instance_id, TaskManager *owning_manager);
    
    //TimedAlgorithmInstance(const TimedAlgorithmInstance& orig);
    virtual ~TimedTaskInstance();

    //void emplaceToCalendar();
    
    // Plans to calendar when this instance shoud be activated again.
    void planActivationAfterSeconds(int seconds);
    
    // Plans to calendar activation of and instance to present time.
    void planActivationNow();
    
    // Activates an instance (calls run() function, but protects it with mutex).
    void activate(std::chrono::system_clock::time_point activation_time) override;
    
    void removeFromCalendar();

    // Entry point of instance.
    virtual void run(std::chrono::system_clock::time_point activation_time) = 0;
    
private:
    std::set<std::chrono::system_clock::time_point> m_activation_times;
    
    std::mutex m_activation_mx;

};

#endif /* TIMEDTASKMINSTANCE_H */

