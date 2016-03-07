/* 
 * File:   TimedTaskManager.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 20. January 2016
 */

#ifndef TIMEDTASKMANAGER_H
#define TIMEDTASKMANAGER_H

#include <map>
#include <memory>
#include <string>

#include "TaskManager.h"
#include "TimedTaskInstance.h"

class TimedTaskManager : public TaskManager
{
public:
    
    TimedTaskManager(MANAGER_TYPE type, std::string name);
    
    //TimedAlgorithmManager(const TimedAlgorithmManager& orig);
    
    virtual ~TimedTaskManager();
    
    // Creates an instance of algorithm.
    virtual void createInstance(unsigned long user_id, unsigned int users_instance_personal_id);
    
protected:

    // Map containing instance_id as a key, and pointer to an object of an instance of an algorithm.
    std::map<unsigned long /*instance_id*/, std::shared_ptr<TimedTaskInstance>> m_algorithm_instances;
};

#endif /* TIMEDALGORITHMMANAGER_H */

