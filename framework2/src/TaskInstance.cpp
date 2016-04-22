/* 
 * File:   TaskInstance.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 20. January 2016
 */

#include "TaskInstance.h"

#include <string>
#include <stdexcept>

#include "Logger.h"
#include "TaskManager.h"


TaskInstance::TaskInstance(long instance_id, std::weak_ptr<TaskManager> owning_manager):
    m_instance_id(instance_id),
    m_owning_manager(owning_manager)
{
}

TaskInstance::~TaskInstance()
{
}

void TaskInstance::deleteItself()
{
    try {
        if (auto locked = m_owning_manager.lock()) {
            // Suicide instance.
            locked->suicideInstance(m_instance_id);
        }
    }
    catch (const std::exception& e) {
        logger.LOGFILE("task_instance", "ERROR") << e.what() << std::endl;
    }
}
