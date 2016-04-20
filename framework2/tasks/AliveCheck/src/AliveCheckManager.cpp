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
    std::shared_ptr<TaskManager> createTaskManager() {
        
        std::cout << "Creating AliveCheckManager." << std::endl;
        //return new AliveCheckManager();
        std::shared_ptr<TaskManager> manager = std::make_shared<AliveCheckManager>();
        std::cout << "AliveCheckManager created." << std::endl;
        return manager;
    }
    
    void deleteTaskManager(TaskManager* manager) {
        std::cout << "Deleting AliveCheckManager." << std::endl;
        
        manager->debugPrintTaskInstances();
        /*
        AliveCheckManager* alive_check_manager = reinterpret_cast<AliveCheckManager*>(manager);
        if (alive_check_manager != nullptr) {
            delete alive_check_manager;
        }
         * */
        std::cout << "AliveCheckManager deleted." << std::endl;
    }
}

AliveCheckManager::AliveCheckManager()
{
}

AliveCheckManager::~AliveCheckManager()
{
    std::cout << "AliveCheckManager::~AliveCheckManager" << std::endl;
}

void AliveCheckManager::createConfiguration(long instance_id, std::map<std::string, std::string> config)
{
    std::cout << "AliveCheckManager::createConfiguration - enter" << std::endl;
    
    AliveCheckConfig alive_config = parseConfiguration(config);
    
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    *sql << "INSERT INTO task_alive_check(instance_id, send_notif, gateway_id) VALUES(:instance_id, :send_notif, :gateway_id)",
            soci::use(instance_id, "instance_id"), soci::use(alive_config.gateway_id, "gateway_id"),
            soci::use(alive_config.send_notif, "send_notif");
    
    std::cout << "Emplace instance with instance_id: " << instance_id << " into BAF." << std::endl;
    
    std::lock_guard<std::mutex> lock(m_task_instances_mx);
    m_task_instances.emplace(instance_id, std::make_shared<AliveCheckInstance>(instance_id, shared_from_this()));
    
    debugPrintTaskInstances();
    
    std::cout << "AliveCheckManager::createConfiguration - leave" << std::endl;
}

void AliveCheckManager::changeConfiguration(ChangeMessage change_message)
{
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    soci::transaction tr(*sql);
    
    auto gateway_id = change_message.config.find("gateway_id");
    if (gateway_id != change_message.config.end()) {
        *sql << "UPDATE task_alive_check SET gateway_id = :gateway_id WHERE instance_id = :instance_id",
                soci::use(std::stoi(gateway_id->second), "gateway_id"),
                soci::use(change_message.instance_id, "instance_id");
    }
    
    auto send_notif = change_message.config.find("send_notif");
    if (send_notif != change_message.config.end()) {
        *sql << "UPDATE task_alive_check SET send_notif = :send_notif WHERE instance_id = :instance_id",
                soci::use(std::stoi(send_notif->second), "send_notif"),
                soci::use(change_message.instance_id, "instance_id");
    }
    tr.commit();
}

std::map<std::string, std::string> AliveCheckManager::getConfiguration(GetConfMessage get_conf_message)
{
    AliveCheckConfig alive_config;
    std::map<std::string, std::string> config_map;
    
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    *sql << "SELECT gateway_id, send_notif FROM task_alive_check WHERE instance_id = :instance_id",
            soci::use(get_conf_message.instance_id, "instance_id"),
            soci::into(alive_config.gateway_id),
            soci::into(alive_config.send_notif);

    config_map["gateway_id"] = std::to_string(alive_config.gateway_id);
    config_map["send_notif"] = std::to_string(alive_config.send_notif);
    return config_map;
}

void AliveCheckManager::reloadInstances(unsigned int task_id)
{
    int instance_id;
    
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    
    soci::rowset<soci::row> rows = (sql->prepare << "SELECT instance_id FROM instance WHERE task_id = :task_id",
                                    soci::use(task_id, "task_id"));

    std::lock_guard<std::mutex> lock(m_task_instances_mx);
    for (soci::rowset<soci::row>::const_iterator it = rows.begin(); it != rows.end(); ++it) {   
        
        soci::row const& row = *it;
        instance_id = row.get<int>(0);
        
        m_task_instances.emplace(instance_id, std::make_shared<AliveCheckInstance>(instance_id, shared_from_this()));

        
        std::cout << "Reload instance with instance_id: " << instance_id << " into BAF." << std::endl;
    }
    
}

AliveCheckConfig AliveCheckManager::parseConfiguration(std::map<std::string, std::string> configuration)
{
    AliveCheckConfig conf;
    conf.gateway_id = std::stoi(configuration.find("gateway_id")->second);
    conf.send_notif = std::stoi(configuration.find("send_notif")->second);
    return conf;
}
