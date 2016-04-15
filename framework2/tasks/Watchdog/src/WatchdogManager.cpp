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
    TaskManager* createTaskManager() {
        
        std::cout << "Creating RefreshTaskManager." << std::endl;
        return static_cast<TaskManager*>(new WatchdogManager());
    }
    
    void deleteTaskManager(TaskManager* manager) {
        std::cout << "Deleting WatchdogManager." << std::endl;
        delete static_cast<WatchdogManager*>(manager);
        std::cout << "WatchdogManager deleted." << std::endl;
    }
}

WatchdogManager::WatchdogManager()
{
}

WatchdogManager::~WatchdogManager()
{
}

void WatchdogManager::createConfiguration(long instance_id, std::map<std::string, std::string> config)
{
    WatchdogConfig watchdog_config = parseConfiguration(config);
    
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    *sql << "INSERT INTO task_watchdog(instance_id, device_euid, module_id, operator, value, notification_text)"
            "VALUES(:instance_id, :device_euid, :module_id, :operator, :value, :notification_text)",
            soci::use(instance_id, "instance_id"), soci::use(watchdog_config.device_euid, "device_euid"),
            soci::use(watchdog_config.module_id, "module_id"), soci::use(watchdog_config.comp_operator, "operator"),
            soci::use(watchdog_config.value, "value"), soci::use(watchdog_config.notification_text, "notification_text");
    
    std::cout << "Emplace instance with instance_id: " << instance_id << " into BAF." << std::endl;
    
    std::lock_guard<std::mutex> lock(m_task_instances_mx);
    m_task_instances.emplace(instance_id, std::make_shared<WatchdogInstance>(instance_id, this, watchdog_config.device_euid));

    
    debugPrintTaskInstances();
}

void WatchdogManager::changeConfiguration(ChangeMessage change_message)
{
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    soci::transaction tr(*sql);
    
    auto device_euid = change_message.config.find("device_euid");
    if (device_euid != change_message.config.end()) {
        *sql << "UPDATE task_watchdog SET device_euid = :device_euid", soci::use(std::stol(device_euid->second), "device_euid");
    }
    
    auto module_id = change_message.config.find("module_id");
    if (module_id != change_message.config.end()) {
        *sql << "UPDATE task_watchdog SET module_id = :module_id", soci::use(std::stoi(module_id->second), "module_id");
    }
    
    auto comp_operator = change_message.config.find("operator");
    if (comp_operator != change_message.config.end()) {
        *sql << "UPDATE task_watchdog SET operator = :operator", soci::use(comp_operator->second, "operator");
    }
    
    auto value = change_message.config.find("value");
    if (value != change_message.config.end()) {
        *sql << "UPDATE task_watchdog SET value = :value", soci::use(std::stod(value->second), "value");
    }
    
    auto notification_text = change_message.config.find("notification_text");
    if (notification_text != change_message.config.end()) {
        *sql << "UPDATE task_watchdog SET notification_text = :notification_text", soci::use(notification_text->second, "notification_text");
    }
    
    tr.commit();
}

std::map<std::string, std::string> WatchdogManager::getConfiguration(GetConfMessage get_conf_message)
{
    WatchdogConfig watchdog_config;
    std::map<std::string, std::string> config_map;
    
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    
    *sql << "SELECT device_euid, module_id, operator, value, notification_text"
            "FROM task_watchdog WHERE instance_id = :instance_id",
            soci::use(get_conf_message.instance_id, "instance_id"), soci::into(watchdog_config.device_euid),
            soci::into(watchdog_config.module_id), soci::into(watchdog_config.comp_operator),
            soci::into(watchdog_config.value), soci::into(watchdog_config.notification_text);

    config_map["device_euid"] = std::to_string(watchdog_config.device_euid);
    config_map["module_id"] = std::to_string(watchdog_config.module_id);
    config_map["operator"] = watchdog_config.comp_operator;
    config_map["value"] = std::to_string(watchdog_config.value);
    config_map["notification_text"] = watchdog_config.notification_text;
    
    return config_map;
}

void WatchdogManager::reloadInstances(int task_id)
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
        
        m_task_instances.emplace(instance_id, std::make_shared<WatchdogInstance>(instance_id, this, device_euid));
        
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
    conf.notification_text = configuration.find("notification_text")->second;
    return conf;
}
