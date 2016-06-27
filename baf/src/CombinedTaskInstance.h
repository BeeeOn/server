/* 
 * File:   CombinedTaskInstance.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 22. January 2016
 */

#ifndef COMBINEDTASKINSTANCE_H
#define COMBINEDTASKINSTANCE_H

#include "TimedTaskInstance.h"
#include "TriggerTaskInstance.h"

class CombinedTaskInstance: public TimedTaskInstance, public TriggerTaskInstance
{
public:
    /**
     * Constructor of class CombinedTaskInstance.
     */
    CombinedTaskInstance(long instance_id, std::weak_ptr<TaskManager> owning_manager);

    /**
     * Destructor of class CombinedTaskInstance.
     */
    virtual ~CombinedTaskInstance();
    
    /**
     * Removes all entries from DataMessageRegister and Calendar of instance.
     * After this instance won't receive any data messages or activations from Calendar.
     */
    void deleteFromControlComponent() override; 
    /**
     * Removes all planned activations of instance from Calendar.
     */
    void deleteFromCalendar();
    /**
     * Removes all entries from DataMessageRegister.
     */
    void deleteFromDataMessageRegister();
};

#endif /* COMBINEDTASKINSTANCE_H */
