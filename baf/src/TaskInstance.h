/* 
 * File:   TaskInstance.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 20. January 2016
 */

#ifndef TASKINSTANCE_H
#define TASKINSTANCE_H

#include <chrono>
#include <map>
#include <memory>
#include <mutex>

#include "DataMessage.h"

class TaskManager;

class TaskInstance
{
public:
    /**
     * Constuctor of class TaskInstance.
     * @param instance_id Identificator of created instance.
     * @param owning_manager Pointer to manager of this instance.
     */
    TaskInstance(long instance_id, std::weak_ptr<TaskManager> owning_manager);
    /**
     * Destructor of class TaskInstance.
     */
    virtual ~TaskInstance();
    /**
     * Virtual function used for activating Timed instance.
     */
    virtual void activate(std::chrono::system_clock::time_point activation_time) {};
    /**
     * Virtual function to activate Trigger instance.
     */
    virtual void activate(DataMessage data_message) {};
    /**
     * Getter for instance_id.
     * @return ID of this instance.
     */
    long getInstanceId() { return m_instance_id; };
    /**
     * Removes all entries of instance from control component (Calendar, DataMessageRegister).
     */
    virtual void deleteFromControlComponent() = 0;
    
protected:
    /**
     * Suicide function. Deletes instance from its manager and from database.
     */
    void deleteItself();
    /**
     * Unique ID of this instance from database.
     */
    long m_instance_id;
    /**
     * Pointer to manager of instance.
     */
    std::weak_ptr<TaskManager> m_owning_manager;
    /**
     * Mutex which protects instance from being activated more than once.
     */
    std::mutex m_activation_mx;
 };

#endif /* TASKINSTANCE_H */
 