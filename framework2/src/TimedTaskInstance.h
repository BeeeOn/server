/* 
 * File:   TimedTaskInstance.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 22. January 2016
 */

#ifndef TIMEDTASKINSTANCE_H
#define TIMEDTASKINSTANCE_H

#include <memory>
#include <mutex>
#include <string>

#include "TaskInstance.h"


class TimedTaskInstance: public TaskInstance//, std::enable_shared_from_this<TimedTaskInstance>
{
public:

    
    TimedTaskInstance(unsigned int instance_id);
    
    //TimedAlgorithmInstance(const TimedAlgorithmInstance& orig);
    virtual ~TimedTaskInstance();

    //void emplaceToCalendar();
    
    // Plans to calendar when this instance shoud be activated again.
    void planActivationAfterSeconds(int seconds);
    
    // Plans to calendar activation of and instance to present time.
    void planActivationNow();
    
    // Activates an instance (calls run() function, but protects it with mutex).
    void activate() override;
    
    void deleteInstance() override;

    
    // Entry point of instance.
    virtual void run() = 0;
    
private:
    std::mutex m_activation_mx;

};

#endif /* TIMEDTASKMINSTANCE_H */

