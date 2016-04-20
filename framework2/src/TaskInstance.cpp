/* 
 * File:   TaskInstance.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 20. January 2016
 */

#include "TaskInstance.h"

#include <string>

#include "TaskManager.h"
#include "UserMessages.h"

TaskInstance::TaskInstance(int instance_id, std::weak_ptr<TaskManager> owning_manager):
    m_instance_id(instance_id),
    m_owning_manager(owning_manager)
{
}

TaskInstance::~TaskInstance()
{
}

void TaskInstance::deleteItself()
{
    if (auto locked = m_owning_manager.lock()) {
        locked->suicideInstance(m_instance_id);
    }
}
