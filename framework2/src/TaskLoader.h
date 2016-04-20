/* 
 * File:   TaskLoader.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 20. January 2016
 */

#ifndef TASKLOADER_H
#define TASKLOADER_H

#include <map>
#include <memory>
#include <stdexcept>
#include <string>

#include "pugixml.hpp"

#include "BaseTaskLoader.h"
#include "Task.h"

class TaskLoader: public BaseTaskLoader
{
public:

    /**
     * Destructor of class TaskLoader.
     */
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
    /**
     * Reads again tasks config file and loads tasks with new ids.
     */
    void reloadTasksConfigFileAndFindNewTasks();
    
protected:
    /** 
     * Private constructor of class TaskLoader.
     */
    TaskLoader();
    /**
     * Delete copy constructor.
     */
    TaskLoader(const TaskLoader& orig) = delete;
    /**
     * Delete assignment operator.
     */
    void operator=(const TaskLoader&) = delete;
};

#endif /* TASKLOADER_H */
