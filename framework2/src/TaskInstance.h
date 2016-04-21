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
     * @param user_id ID of a user creating an instance.
     * @param personal_id ID of instance relative to instances of task already created by user (user can run more instances of one task).
     */
    //TaskInstance(int instance_id, TaskManager* owning_manager);
    TaskInstance(int instance_id, std::weak_ptr<TaskManager> owning_manager);
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
    unsigned int getInstanceId() { return m_instance_id; };
    /**
     * Removes all entries of instance from control component (Calendar, DataMessageRegister).
     */
    virtual void deleteFromControlComponent() = 0;
    /**
     * Suicide function. Deletes instance from baf and from database.
     */
    void deleteItself();
    
protected:
    /**
     * Unique ID of this instance in database (and in BAF system -> key value in map of manager)
     */
    int m_instance_id;
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
 