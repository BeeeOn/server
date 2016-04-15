/* 
 * File:   TaskManager.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 20. January 2016
 */

#include "TaskManager.h"

#include <iostream>
#include <sstream>
#include <stdexcept>

#include "DatabaseInterface.h"
#include "Logger.h"
#include "TaskLoader.h"

TaskManager::TaskManager()
{
}

TaskManager::~TaskManager()
{
}

long TaskManager::createInstance(CreateMessage create_message)
{
    long instance_id;
    std::string permission("superuser");
    
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    soci::transaction tr(*sql);
    
    *sql << "INSERT INTO instance(task_id) VALUES (:task_id) RETURNING instance_id",
            soci::into(instance_id), soci::use(create_message.task_id);
    
    *sql << "INSERT INTO user_instance(user_id, instance_id, permission) VALUES(:user_id, :instance_id, :permission)",
            soci::use(create_message.user_id, "user_id"), soci::use(instance_id, "instance_id"), soci::use(permission, "permission");
     
    tr.commit();
    
    return instance_id;
}

void TaskManager::deleteInstance(DeleteMessage delete_message)
{
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    
    *sql << "DELETE FROM instance WHERE instance_id = :instance_id",
            soci::use(delete_message.instance_id, "instance_id");
    
    // Get task type of task from delete message.
    //TASK_TYPE task_type = TaskLoader::getInstance()->m_tasks.find(delete_message.task_id)->second->m_task_type;
    
    
    // Delete from calendar.
    // Obsolete, removing from calendar is handled in timed instance destructor.
    // m_task_instances.find(delete_message.instance_id)->second->removeFromCalendar();
    
    std::lock_guard<std::mutex> lock(m_task_instances_mx);
    m_task_instances.erase(delete_message.instance_id);
    
    debugPrintTaskInstances(); 
}

std::vector<std::string> TaskManager::getInstanceIds(GetInstIdsMessage get_inst_ids_message)
{
    std::cout << "TaskManager::getInstanceIds - enter" << std::endl; 
    std::vector<std::string> instance_ids(100);
    
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    *sql << "SELECT instance.instance_id "
            "FROM user_instance INNER JOIN instance "
            "ON user_instance.instance_id = instance.instance_id "
            "WHERE task_id = :task_id AND user_id = :user_id",
            soci::into(instance_ids),
            soci::use(get_inst_ids_message.task_id, "task_id"),
            soci::use(get_inst_ids_message.user_id, "user_id");
    
    
    std::cout << "TaskManager::getInstanceIds - leave" << std::endl; 
    
    return instance_ids;
}

void TaskManager::debugPrintTaskInstances()
{
    std::string task_instances;
    if (m_task_instances.size() == 0) {
        task_instances += "none";
    }
    else {
        for (auto task_instance : m_task_instances) {
        task_instances += std::to_string(task_instance.first);
        task_instances += ", ";
        }
    }
    
    logger.LOGFILE("task_manager", "INFO") << "Task instances: " << task_instances << std::endl;
}

void TaskManager::suicideInstance(long instance_id)
{
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    
    *sql << "DELETE FROM instance WHERE instance_id = :instance_id",
            soci::use(instance_id, "instance_id");
    
    std::lock_guard<std::mutex> lock(m_task_instances_mx);
    m_task_instances.erase(instance_id);
    
    debugPrintTaskInstances(); 
}


