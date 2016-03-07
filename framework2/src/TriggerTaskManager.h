/* 
 * File:   TriggerTaskManager.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 20. January 2016
 */

#ifndef TRIGGERTASKMANAGER_H
#define TRIGGERTASKMANAGER_H

#include "TaskManager.h"

class TriggerTaskManager : public TaskManager
{
public:
    
    TriggerTaskManager(MANAGER_TYPE type, std::string name);
    
    //EventAlgorithmManager(const EventAlgorithmManager& orig);
    
    virtual ~TriggerTaskManager();
    
    virtual void processDataMessage();
    
private:

};

#endif /* TRIGGERTASKMANAGER_H */

