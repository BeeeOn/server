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
#include <set>

class TriggerTaskInstance: public TaskInstance 
{
public:
    TriggerTaskInstance(unsigned int instance_id, TaskManager *owning_manager);
    
    //TriggerTaskInstance(const TriggerTaskInstance& orig);
    
    virtual ~TriggerTaskInstance();

    void activate(DataMessage data_message) override;
    
    // Entry point of instance.
    virtual void run(DataMessage data_message) = 0;

    void registerDataMessage(long device_euid);
    
    void removeFromDataMessageRegister();
    
private:

    std::set<long /*device_euid*/> m_registered_device_euids;
    
    std::mutex m_activation_mx;
};

#endif /* TRIGGERTASKINSTANCE_H */

