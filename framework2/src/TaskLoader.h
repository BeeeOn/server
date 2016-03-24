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
    
    /**
     * Returns pointer to singleton instance of class DatabaseInterface.
     * @return Pointer to singleton instance.
     */
    static std::shared_ptr<TaskLoader> getInstance();
    
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
    
    /**
     * Finds task in m_tasks and returns shared pointer to it.
     * @param task_id ID of a task.
     * @return Shared pointer to task.
     */
    std::shared_ptr<Task> findTask(unsigned int task_id);
    
private:
    /**
     * Singleton instance pointer.
     */
    static std::shared_ptr<TaskLoader> m_instance;
    
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

