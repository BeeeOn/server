/* 
 * File:   Task.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 7. March 2016
 */

#ifndef TASK_H
#define TASK_H

#include <sstream>
#include <string>
#include <stdexcept>
#include <memory>

#include <dlfcn.h>

#include "TaskManager.h"

enum class TASK_TYPE {
    TIMED, TRIGGER, COMBINED
};

class Task
{
public:
    /**
     * Task class constructor.
     * @param task_version Version of this task.
     * @param task_name Name of this task.
     * @param task_type Type of this task.
     * @param task_path Path to library file (.so) with this task.
     */
    Task(unsigned short task_version, std::string task_name, TASK_TYPE task_type, std::string task_path);
    /**
     * Task class destructor.
     */
    virtual ~Task();
    /**
     * Opens tasks dynamic library via dlopen() by path stored in m_task_path.
     */
    void openTaskLibrary();
    /*
     * Closes tasks dynamic library via dlclose().
     */
    void closeTaskLibrary();
    /**
     * Creates new task manager. Type depends on type in m_task_type.
     */
    void createTaskManager();
    
private:
    /**
     * Pointer to library (.so file) opened by dlopen().
     * Using raw void pointer, because dlopen loads it in C way.
     */
    void* m_task_library;
    /**
     * Manager of loaded task.
     */
    std::shared_ptr<TaskManager> m_task_manager;
    /**
     * Version of this task.
     */
    unsigned short m_task_version;
    /**
     * Name of this task.
     */
    std::string m_task_name;
    /**
     * Type of this task. Can be TIMED, TRIGGER, COMBINED.
     */
    TASK_TYPE m_task_type;
    /**
     * Path to dynamic library file of this task (.so).
     */
    std::string m_task_path;
    
public:
    /**
     * Getter for task manager.
     * @return Shared pointer to task
     */
    std::shared_ptr<TaskManager> getTaskManagerPtr() { return m_task_manager; }
};

#endif /* TASK_H */
