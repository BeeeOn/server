/* 
 * File:   TriggerTaskInstance.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 22. January 2016
 */

#ifndef TRIGGERTASKINSTANCE_H
#define TRIGGERTASKINSTANCE_H

#include "DataMessage.h"
#include "TaskInstance.h"

#include <mutex>

class TriggerTaskInstance: public TaskInstance 
{
public:
    TriggerTaskInstance(unsigned int instance_id);
    
    TriggerTaskInstance(const TriggerTaskInstance& orig);
    
    virtual ~TriggerTaskInstance();

    void activate(DataMessage data_message) override;
    
    // Entry point of instance.
    virtual void run(DataMessage data_message) = 0;

    void registerDataMessage(unsigned long long device_euid);
    
private:
        
    std::mutex m_activation_mx;
};

#endif /* TRIGGERTASKINSTANCE_H */

