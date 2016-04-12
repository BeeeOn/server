/* 
 * File:   BaseTaskLoader.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 12. April 2016
 */

#include "BaseTaskLoader.h"

#include <iostream>
#include <stdexcept>

std::shared_ptr<BaseTaskLoader> BaseTaskLoader::m_instance;

BaseTaskLoader::BaseTaskLoader()
{
}

BaseTaskLoader::~BaseTaskLoader()
{
}

std::shared_ptr<Task> BaseTaskLoader::findTask(unsigned int task_id)
{
    auto found = m_tasks.find(task_id);
    
    if (found != m_tasks.end()) {
        std::cout << "Task with id: " << task_id << " was found." << std::endl;
        return found->second;
    }
    else {
        // Task was not found.
        std::stringstream error;
        error << "Task with id: " << task_id << " was not found in the BAF system.";
        throw std::runtime_error(error.str());
    }
}


std::shared_ptr<BaseTaskLoader> BaseTaskLoader::getInstance()
{
    //std::cout << "TaskLoader::getInstance()" << std::endl;
    
    if (!m_instance) {
        
        throw std::runtime_error("Instance of TaskLoader must be created first - createInstance() function.");
    }
    else {
        return m_instance;
    }
}