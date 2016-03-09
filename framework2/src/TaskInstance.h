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
    TaskInstance(unsigned long user_id, unsigned int users_instance_personal_id);
    
    ~TaskInstance();
   
    /*
     * Pure virtual function used for activating
     */
    virtual void activate() = 0;
    
    
    //AlgorithmInstance(const AlgorithmInstance& orig);
    
    //virtual ~AlgorithmInstance();
    
protected:
    // There can be multiple owners of one instance.
    // First is user_id from database, second is users personal users_instance_personal_id
    // (example: user can have a several running instances of one algorithm).
    // THERE SHOULD BE ROLES FOR INSTANCES (OWNER? VIEWER?).
    std::map<unsigned long /*user_id*/, unsigned int /*users_instance_personal_id*/> m_owners;
};

#endif /* TASKINSTANCE_H */