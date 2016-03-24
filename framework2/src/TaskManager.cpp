/* 
 * File:   AlgorithmManager.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 20. January 2016
 */

#include "TaskManager.h"

#include <iostream>
#include <sstream>
#include <stdexcept>

#include "DatabaseInterface.h"


TaskManager::TaskManager()
{
}

TaskManager::~TaskManager()
{
}

bool TaskManager::checkInstanceExistence(ConfigMessage config_message)
{
    // 
    unsigned int instance_id;
    
    try {
        instance_id = getInstanceId(config_message.user_id, config_message.relative_id);
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return false;
    }
    
    // If found in database, check instance container to be sure it exists.
    auto found = m_task_instances.find(instance_id);
    if (found != m_task_instances.end()) {
        return true;
    }
    else {
        // Inconsistency of BAF and database should be resolved.
        return false;
    }
}

unsigned int TaskManager::getInstanceId(unsigned int user_id, unsigned short relative_id) {
    
    unsigned int instance_id;
    
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    soci::indicator ind;
    
    // Find instance in database.
    *sql << "SELECT instance_id FROM user_instance WHERE user_id = :user_id AND relative_id = :relative_id",
            soci::into(instance_id, ind), soci::use(user_id, "user_id"), soci::use(relative_id, "relative_id");

    if (!sql->got_data() || (ind != soci::i_ok)) {
        // If not found false.
        std::stringstream error;
        error << "Instance of user: " << user_id << " with relative_id: " << relative_id;
        throw std::runtime_error(error.str());
    }
    return instance_id;
}

void TaskManager::makeNewInstance(ConfigMessage config_message) {
    
    unsigned int instance_id;
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    *sql << "INSERT INTO instance(task_id) VALUES (:task_id) RETURNING instance_id",
            soci::into(instance_id), soci::use(config_message.task_id);
    
    *sql << "INSERT INTO user_instance(user_id, instance_id, relative_id) VALUES(:user_id, :instance_id, :relative_id)",
            soci::use(config_message.user_id, "user_id"), soci::use(instance_id, "instance_id"), soci::use(config_message.relative_id, "relative_id");
     
    createInstance(instance_id, config_message.parameters);
    
}

void TaskManager::deleteInstance(unsigned int user_id, unsigned short personal_id) {

}
