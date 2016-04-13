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

#include "BaseTaskLoader.h"
#include "Task.h"


//#include "TasksConfigParser.h"
//#include "TriggerTaskManager.h"
//#include "TimedTaskManager.h"

class TaskLoader: public BaseTaskLoader
{
public:
    /**
     * Delete copy constructor.
     */
    TaskLoader(const TaskLoader& orig) = delete;
    
    /**
     * Delete assignment operator.
     */
    void operator=(const TaskLoader&) = delete;
    
    virtual ~TaskLoader();
    
    /**
     * Returns pointer to singleton instance of class DatabaseInterface.
     * @return Pointer to singleton instance.
     */
    //static std::shared_ptr<TaskLoader> getInstance();
    static void createInstance();
    
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
    
    void reloadTasksConfigFileAndFindNewTasks();
    
    
    /**
     * Finds task in m_tasks and returns shared pointer to it.
     * @param task_id ID of a task.
     * @return Shared pointer to task.
     */
    //std::shared_ptr<Task> findTask(unsigned int task_id);
    
protected:
    /** 
     * Private constructor.
     */
    TaskLoader();
   
    //std::string m_tasks_config_file_path;
    
   
    /**
     * Singleton instance pointer.
     */
    //static std::shared_ptr<TaskLoader> m_instance;
    
    /**
     * Container storing all loaded tasks.
     */
    //std::map<unsigned int /* ID of task */, std::shared_ptr<Task>> m_tasks;

};

#endif /* TASKLOADER_H */

