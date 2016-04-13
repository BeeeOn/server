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

TaskInstance::TaskInstance(int instance_id, TaskManager* owning_manager):
    m_instance_id(instance_id),
    m_owning_manager(owning_manager)
{
}

TaskInstance::~TaskInstance()
{
}

void TaskInstance::deleteItself()
{
    m_owning_manager->suicideInstance(m_instance_id);
}
