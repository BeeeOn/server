/* 
 * File:   Task.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 7. March 2016
 */

#include "Task.h"

#include "Logger.h"

Task::Task(unsigned short task_version, std::string task_name, TASK_TYPE task_type, std::string task_path):
    m_task_version(task_version), m_task_name(task_name), m_task_type(task_type), m_task_path(task_path),
    m_task_library(nullptr), m_task_manager(nullptr)
{
}

Task::~Task()
{
    //closeTaskLibrary();
}

void Task::openTaskLibrary()
{
    // Open dynamic library by path.
    m_task_library = dlopen(m_task_path.c_str(), RTLD_NOW); //RTLD_NOW
    
    if (!m_task_library) {
        // Was not successfuly opened.
        std::stringstream error;
        error << "dlopen(): Failed to open dynamic library of task: " << m_task_name << ". Path: " << m_task_path << std::endl;
        throw std::runtime_error(error.str());
    }
    else {
        logger.LOGFILE("task_loader", "INFO") << "dlopen(): Dynamic library of task "
                << m_task_name << " was successfuly opened." << std::endl;
        // Resets errors in dlfcn library.
        dlerror();
    }
}

void Task::closeTaskLibrary()
{
    if (dlclose(m_task_library) != 0) {
        logger.LOGFILE("task_loader", "ERROR") << "dlclose(): Dynamic library of task "
                << m_task_name << " couldn't be closed." << std::endl;
    }
    else {
        logger.LOGFILE("task_loader", "INFO") << "dlclose(): Dynamic library of task "
                << m_task_name << " was successfuly closed." << std::endl;
    }
    
}

void Task::createTaskManager()
{
    // Function pointer.
    typedef std::shared_ptr<TaskManager>(*create_task_manager)();
    
    if (!m_task_library) {
        std::stringstream error;
        error << "Cannot create task manager of task: " << m_task_name << ", because it's dynamic library wasn't opened." << std::endl;
        throw std::runtime_error(error.str());
    }
    else {
        // Load symbol of createTaskManager function.
        create_task_manager createTaskManager = (create_task_manager)dlsym(m_task_library, "createTaskManager");
    
        const char* dlsym_error = dlerror();
        if (dlsym_error) {
            // Loading of function symbol was not successful.
            std::stringstream error;
            error << "dlsym(): Cannot load createTaskManager function symbol of task: " << m_task_name << std::endl;
            throw std::runtime_error(error.str());
        }
        else {
            
            m_task_manager = createTaskManager();
        }
    }
}