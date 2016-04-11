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
#include <string>

#include "DataMessage.h"
#include "TaskManager.h"

class TaskManager;
class TaskInstance//: public std::enable_shared_from_this<TaskInstance>
{
public:
    /*
    std::shared_ptr<TaskInstance> weakFromThis()
    {
       return shared_from_this();
    }
    */
    /**
     * Constuctor of class TaskInstance.
     * @param user_id ID of a user creating an instance.
     * @param personal_id ID of instance relative to instances of task already created by user (user can run more instances of one task).
     */
    TaskInstance(unsigned int instance_id, TaskManager *owning_manager);
    
    /**
     * Destructor of class TaskInstance.
     */
    virtual ~TaskInstance();
   
    /**
     * Virtual function used for activating Timed instance (eventualy Combined instances).
     */
    virtual void activate(std::chrono::system_clock::time_point activation_time) {};
    
    /**
     * Virtual function to activate Trigger instance.
     */
    virtual void activate(DataMessage data_message) {};
    
    
    unsigned int getInstanceId() { return m_instance_id; };    

protected:
    
    /** Unique ID of this instance in database (and in BAF system -> key value in map of manager) */
    unsigned int m_instance_id;
    
    /** Pointer to manager of instance. */
    TaskManager* m_owning_manager;
 };

#endif /* TASKINSTANCE_H */
 