/* 
 * File:   ManagerLoader.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 20. January 2016
 */

#ifndef MANAGERLOADER_H
#define MANAGERLOADER_H

#include <map>
#include <memory>

#include <dlfcn.h>

#include "TasksConfigParser.h"
#include "TriggerTaskManager.h"
#include "TimedTaskManager.h"

class ManagerLoader {
public:
    ManagerLoader();
    ManagerLoader(const ManagerLoader& orig);
    virtual ~ManagerLoader();
    
    // Loads algorithm 
    void loadAlgorithmManagers();
        
    //// NEW INTERFACE ////
    
    void loadAllTasks(std::string tasks_config_file);
    
    void loadTimedTask(TaskConfigInfo info);
    
    void closeAllTasks();
    
private:
    //std::map<int /*manager_id*/, AlgorithmManager> algorithm_managers;
    
    //std::map<int /*manager_id*/, std::shared_ptr<TimedTaskManager>> m_timed_managers;
    
    std::map<int /*manager_id*/, TimedTaskManager*> m_timed_managers;
    
    std::map<int /*manager_id*/, TriggerTaskManager> event_managers;
    
    std::vector<TaskConfigInfo> tasks_config_info;
    
    std::map<int /*id*/, void* /*poiner to library*/> loaded_task_libraries;
    
};

#endif /* MANAGERLOADER_H */

