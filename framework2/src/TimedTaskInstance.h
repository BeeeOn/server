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


class TimedTaskInstance: public TaskInstance//, enable_shared_from_this<TimedAlgorithmInstance>
{
public:
    /*std::shared_ptr<TimedAlgorithmInstance> getptr() {
        return shared_from_this();
    }
    */
    
    TimedTaskInstance(unsigned long user_id, unsigned int users_instance_personal_id);
    
    //TimedAlgorithmInstance(const TimedAlgorithmInstance& orig);
    virtual ~TimedTaskInstance();

    //void emplaceToCalendar();
    
    // Plans to calendar when shoud be run again.
    void planActivationAfterSeconds(int seconds);
    
    void planActivationNow();
    
    // Actualy runs code.
    void activate();
    
    virtual void run();
    
private:
    std::mutex m_activation_mx;

};

#endif /* TIMEDTASKMINSTANCE_H */

