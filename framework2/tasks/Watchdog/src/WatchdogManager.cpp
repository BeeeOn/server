/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   WatchdogManager.cpp
 * Author: mrmaidx
 * 
 * Created on 29. března 2016, 12:52
 */

#include "WatchdogManager.h"

#include <map>
#include <mutex>
#include <string>
#include <iostream>

#include <soci.h>

#include "../../../src/DatabaseInterface.h"

#include "WatchdogInstance.h"

extern "C" {
    std::shared_ptr<TaskManager> createTaskManager() {
        
        std::cout << "Creating WatchdogManager." << std::endl;

        std::shared_ptr<TaskManager> manager = std::make_shared<WatchdogManager>();
        std::cout << "WatchdogManager created." << std::endl;
        return manager;
    }
    
    void deleteTaskManager(TaskManager* manager) {
        std::cout << "Deleting WatchdogManager." << std::endl;
        
        manager->debugPrintTaskInstances();
        
        
        std::cout << "WatchdogManager deleted." << std::endl;
    }
}

WatchdogManager::WatchdogManager()
{
}

WatchdogManager::~WatchdogManager()
{
    std::cout << "WatchdogManager::~WatchdogManager" << std::endl;
}

void WatchdogManager::createConfiguration(long instance_id, std::map<std::string, std::string> config)
{
    WatchdogConfig watchdog_config = parseConfiguration(config);
    
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    *sql << "INSERT INTO task_watchdog(instance_id, device_euid, module_id, operator, value, notification)"
            "VALUES(:instance_id, :device_euid, :module_id, :operator, :value, :notification)",
            soci::use(instance_id, "instance_id"), soci::use(watchdog_config.device_euid, "device_euid"),
            soci::use(watchdog_config.module_id, "module_id"), soci::use(watchdog_config.comp_operator, "operator"),
            soci::use(watchdog_config.value, "value"), soci::use(watchdog_config.notification, "notification");
    
    std::cout << "Emplace instance with instance_id: " << instance_id << " into BAF." << std::endl;
    
    std::lock_guard<std::mutex> lock(m_task_instances_mx);
    m_task_instances.emplace(instance_id, std::make_shared<WatchdogInstance>(instance_id, shared_from_this(), watchdog_config.device_euid));

    
    debugPrintTaskInstances();
}

void WatchdogManager::changeConfiguration(ChangeMessage change_message)
{
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    soci::transaction tr(*sql);
    
    auto device_euid = change_message.config.find("device_euid");
    if (device_euid != change_message.config.end()) {
        *sql << "UPDATE task_watchdog SET device_euid = :device_euid WHERE instance_id = :instance_id",
                soci::use(std::stol(device_euid->second), "device_euid"),
                soci::use(change_message.instance_id, "instance_id");
    }
    
    auto module_id = change_message.config.find("module_id");
    if (module_id != change_message.config.end()) {
        *sql << "UPDATE task_watchdog SET module_id = :module_id WHERE instance_id = :instance_id",
                soci::use(std::stoi(module_id->second), "module_id"),
                soci::use(change_message.instance_id, "instance_id");
    }
    
    auto comp_operator = change_message.config.find("operator");
    if (comp_operator != change_message.config.end()) {
        *sql << "UPDATE task_watchdog SET operator = :operator WHERE instance_id = :instance_id",
                soci::use(comp_operator->second, "operator"),
                soci::use(change_message.instance_id, "instance_id");
    }
    
    auto value = change_message.config.find("value");
    if (value != change_message.config.end()) {
        *sql << "UPDATE task_watchdog SET value = :value WHERE instance_id = :instance_id",
                soci::use(std::stod(value->second), "value"),
                soci::use(change_message.instance_id, "instance_id");
    }
    
    auto notification = change_message.config.find("notification");
    if (notification != change_message.config.end()) {
        *sql << "UPDATE task_watchdog SET notification = :notification WHERE instance_id = :instance_id",
                soci::use(notification->second, "notification"),
                soci::use(change_message.instance_id, "instance_id");
    }
    
    tr.commit();
}

std::map<std::string, std::string> WatchdogManager::getConfiguration(GetConfMessage get_conf_message)
{
    WatchdogConfig watchdog_config;
    std::map<std::string, std::string> config_map;
    
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    
    *sql << "SELECT device_euid, module_id, operator, value, notification " // It's crucial to have space at the end of string.
            "FROM task_watchdog WHERE instance_id = :instance_id",
            soci::use(get_conf_message.instance_id, "instance_id"), soci::into(watchdog_config.device_euid),
            soci::into(watchdog_config.module_id), soci::into(watchdog_config.comp_operator),
            soci::into(watchdog_config.value), soci::into(watchdog_config.notification);

    config_map["device_euid"] = std::to_string(watchdog_config.device_euid);
    config_map["module_id"] = std::to_string(watchdog_config.module_id);
    config_map["operator"] = watchdog_config.comp_operator;
    config_map["value"] = std::to_string(watchdog_config.value);
    config_map["notification"] = watchdog_config.notification;
    
    return config_map;
}

void WatchdogManager::reloadInstances(unsigned int task_id)
{
    int instance_id;
    long device_euid;
    
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    
    soci::rowset<soci::row> rows = (sql->prepare << "SELECT instance_id FROM instance WHERE task_id = :task_id",
                                    soci::use(task_id, "task_id"));

    std::lock_guard<std::mutex> lock(m_task_instances_mx);
    for (soci::rowset<soci::row>::const_iterator it = rows.begin(); it != rows.end(); ++it) {   
        
        soci::row const& row = *it;
        instance_id = row.get<int>(0);
        
        *sql << "SELECT device_euid FROM task_watchdog WHERE instance_id = :instance_id",
                soci::into(device_euid), soci::use(instance_id, "instance_id");
        
        m_task_instances.emplace(instance_id, std::make_shared<WatchdogInstance>(instance_id, shared_from_this(), device_euid));
        
        std::cout << "Reload instance with instance_id: " << instance_id << " into BAF." << std::endl;
    }
}

WatchdogConfig WatchdogManager::parseConfiguration(std::map<std::string, std::string> configuration)
{
    WatchdogConfig conf;
    conf.device_euid = std::stoull(configuration.find("device_euid")->second);
    conf.module_id = std::stoi(configuration.find("module_id")->second);  
    conf.comp_operator = configuration.find("operator")->second;
    conf.value = std::stod(configuration.find("value")->second);
    conf.notification = configuration.find("notification")->second;
    return conf;
}
