/* 
 * File:   TaskLoader.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 20. January 2016
 */

#ifndef TASKLOADER_H
#define TASKLOADER_H

#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>

#include "pugixml.hpp"

#include "Task.h"

//#include "TasksConfigParser.h"
//#include "TriggerTaskManager.h"
//#include "TimedTaskManager.h"

class TaskLoader {
public:
    TaskLoader();
    
    virtual ~TaskLoader();

    /**
     * Loads and runs all tasks defined in tasks config file.
     * @param tasks_config_file_path Path to tasks config file.
     */
    void createAllTasks(std::string tasks_config_file_path);
    
    /**
     * Processes config file and creates new task entries in m_tasks containter (stores info from config file).
     * @param tasks_config_file_path Path to tasks config file.
     */
    void processTasksConfigFileAndStoreInfo(std::string tasks_config_file_path);
    
//private:
    
    /**
     * Container storing all loaded tasks.
     */
    std::map<unsigned int /* ID of task */, std::shared_ptr<Task>> m_tasks;
    
    
    /////////// OLD //////////////////ú
    //std::map<int /*manager_id*/, AlgorithmManager> algorithm_managers;
    
    //std::map<int /*manager_id*/, std::shared_ptr<TimedTaskManager>> m_timed_managers;
    
    //std::map<int /*manager_id*/, TimedTaskManager*> m_timed_managers;
    
    //std::map<int /*manager_id*/, TriggerTaskManager> event_managers;
    
    //std::vector<TaskConfigInfo> tasks_config_info;
    
    //std::map<int /*id*/, void* /*poiner to library*/> loaded_task_libraries;
   
};

#endif /* TASKLOADER_H */

