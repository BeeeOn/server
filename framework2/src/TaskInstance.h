/* 
 * File:   TaskInstance.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 20. January 2016
 */

#ifndef TASKINSTANCE_H
#define TASKINSTANCE_H

#include <map>
#include <memory>
#include <string>

#include "DataMessage.h"

class TaskInstance//: std::enable_shared_from_this<TaskInstance>
{
public:
    /*
    std::shared_ptr<TaskInstance> sharedFromThis()
    {
        //return shared_from_this();
    }
    */
    /**
     * Constuctor of class TaskInstance.
     * @param user_id ID of a user creating an instance.
     * @param personal_id ID of instance relative to instances of task already created by user (user can run more instances of one task).
     */
    TaskInstance(unsigned int instance_id);
    
    /**
     * Destructor of class TaskInstance.
     */
    ~TaskInstance();
   
    /**
     * Virtual function used for activating Timed instances (eventualy Combined instances).
     */
    virtual void activate() {};
    
    /**
     * Virtual function to activate Trigger instance.
     */
    virtual void activate(DataMessage data_message) {};
    
    
    virtual void deleteInstance() = 0;
    
protected:
    
    /** Unique ID of this instance in database (and in BAF system -> key value in map of manager) */
    unsigned int m_instance_id;
    unsigned int getInstanceId() { return m_instance_id; };
 };

#endif /* TASKINSTANCE_H */