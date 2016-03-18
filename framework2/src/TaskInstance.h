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

class TaskInstance
{
public:
    /**
     * Constuctor of class TaskInstance.
     * @param user_id ID of a user creating an instance.
     * @param personal_id ID of instance relative to instances of task already created by user (user can run more instances of one task).
     */
    TaskInstance(unsigned long user_id, unsigned int personal_id);
    
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
    //virtual void activate(message) {};
 };

#endif /* TASKINSTANCE_H */