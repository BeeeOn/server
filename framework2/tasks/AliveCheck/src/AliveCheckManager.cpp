/* 
 * File:   AliveCheckManager.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 21. January 2016
 */

#include "AliveCheckManager.h"

#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <soci.h>
#include <iostream>

#include "../../../src/DatabaseInterface.h"
#include "AliveCheckInstance.h"

extern "C" {
    TaskManager* createTaskManager() {
        
        std::cout << "Creating AliveCheckManager." << std::endl;
        return static_cast<TaskManager*>(new AliveCheckManager());
    }
    
    void deleteTaskManager(TaskManager* manager) {
        std::cout << "Deleting AliveCheckManager." << std::endl;
        delete static_cast<AliveCheckManager*>(manager);
        std::cout << "Deleting AliveCheckManager." << std::endl;
    }
}

AliveCheckManager::AliveCheckManager()
{
}

AliveCheckManager::~AliveCheckManager()
{
    //std::cout << "RefreshCheckManager::~RefreshCheckManager." << std::endl;
}

void AliveCheckManager::createConfiguration(long instance_id, std::map<std::string, std::string> config)
{
    std::cout << "AliveCheckManager::createConfiguration - enter" << std::endl;
    
    AliveCheckConfig alive_config = parseConfiguration(config);
    
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    *sql << "INSERT INTO task_alive_check(instance_id, notifications, gateway_id) VALUES(:instance_id, :notifications, :gateway_id)",
            soci::use(instance_id, "instance_id"), soci::use(alive_config.gateway_id, "gateway_id"),
            soci::use(alive_config.notifications, "notifications");
    
    std::cout << "Emplace instance with instance_id: " << instance_id << " into BAF." << std::endl;
    
    std::lock_guard<std::mutex> lock(m_task_instances_mx);
    m_task_instances.emplace(instance_id, std::make_shared<AliveCheckInstance>(instance_id, this));
    
    debugPrintTaskInstances();
    
    std::cout << "AliveCheckManager::createConfiguration - leave" << std::endl;
    
}

void AliveCheckManager::changeConfiguration(ChangeMessage change_message)
{
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    soci::transaction tr(*sql);
    
    auto gateway_id = change_message.config.find("gateway_id");
    if (gateway_id != change_message.config.end()) {
        *sql << "UPDATE task_alive_check SET gateway_id = :gateway_id",
                soci::use(std::stoi(gateway_id->second), "gateway_id");
    }
    
    auto notifications = change_message.config.find("notifications");
    if (notifications != change_message.config.end()) {
        *sql << "UPDATE task_alive_check SET notifications = :notifications",
                soci::use(std::stoi(notifications->second), "notifications");
    }
    
    tr.commit();
}

std::map<std::string, std::string> AliveCheckManager::getConfiguration(GetConfMessage get_conf_message)
{
    AliveCheckConfig alive_config;
    std::map<std::string, std::string> config_map;
    
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    *sql << "SELECT gateway_id, notifications FROM task_alive_check WHERE instance_id = :instance_id",
            soci::use(get_conf_message.instance_id, "instance_id"),
            soci::into(alive_config.gateway_id),
            soci::into(alive_config.notifications);

    config_map["gateway_id"] = std::to_string(alive_config.gateway_id);
    config_map["notifications"] = std::to_string(alive_config.notifications);
    return config_map;
}

void AliveCheckManager::reloadInstances(int task_id)
{
    
     
    int instance_id;
    
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    
    soci::rowset<soci::row> rows = (sql->prepare << "SELECT instance_id FROM instance WHERE task_id = :task_id",
                                    soci::use(task_id, "task_id"));

    std::lock_guard<std::mutex> lock(m_task_instances_mx);
    for (soci::rowset<soci::row>::const_iterator it = rows.begin(); it != rows.end(); ++it) {   
        
        soci::row const& row = *it;
        instance_id = row.get<int>(0);
        
        m_task_instances.emplace(instance_id, std::make_shared<AliveCheckInstance>(instance_id, this));

        
        std::cout << "Reload instance with instance_id: " << instance_id << " into BAF." << std::endl;
    }
    
}

AliveCheckConfig AliveCheckManager::parseConfiguration(std::map<std::string, std::string> configuration)
{
    AliveCheckConfig conf;
    conf.gateway_id = std::stoi(configuration.find("gateway_id")->second);
    conf.notifications = std::stoi(configuration.find("notifications")->second);
    return conf;
}
