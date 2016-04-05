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
#include <string>

#include <soci.h>

#include "../../../src/DatabaseInterface.h"

#include "WatchdogInstance.h"

extern "C" {
    TaskManager* createTaskManager() {
        
        std::cout << "Creating RefreshTaskManager." << std::endl;
        return static_cast<TaskManager*>(new WatchdogManager());
    }
    
    void deleteTaskManager(TaskManager* manager) {
        std::cout << "Deleting Manager." << std::endl;
        delete static_cast<WatchdogManager*>(manager);
    }
}

WatchdogManager::WatchdogManager() {
}

WatchdogManager::~WatchdogManager() {
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
    m_task_instances.emplace(instance_id, std::make_shared<WatchdogInstance>(instance_id, watchdog_config.device_euid));
}

void WatchdogManager::changeConfiguration(ChangeMessage change_message)
{
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    soci::transaction tr(*sql);
    
    auto device_euid = change_message.config.find("device_euid");
    if (device_euid != change_message.config.end()) {
        *sql << "UPDATE task_watchdog SET device_euid = :device_euid", soci::use(std::stoull(device_euid->second), "device_euid");
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


/*

void WatchdogManager::createInstance(unsigned int instance_id, std::map<std::string, std::string> configuration) {
    
    std::cout << "Insert instance with instance_id: " << instance_id << " into database." << std::endl;
    WatchdogConfig conf = parseConfiguration(configuration);
    
    // Configuration in special database table.
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    *sql << "INSERT INTO task_watchdog(instance_id, device_euid, module_id, operator, value, notification_text)"
            "VALUES(:instance_id, :device_euid, :module_id, :operator, :value, :notification_text)",
            soci::use(instance_id, "instance_id"), soci::use(conf.device_euid, "device_euid"),
            soci::use(conf.module_id, "module_id"), soci::use(conf.comp_operator, "operator"),
            soci::use(conf.value, "value"), soci::use(conf.notification_text, "notification_text");
            
    std::cout << "Emplace instance with instance_id: " << instance_id << " into BAF." << std::endl;
    m_task_instances.emplace(instance_id, std::make_shared<WatchdogInstance>(instance_id, conf.device_euid));
    
}

void WatchdogManager::updateConfiguration(unsigned int instance_id, std::map<std::string, std::string> configuration)
{
    WatchdogConfig conf = parseConfiguration(configuration);
    // Update configuration of instance with given instance_id.
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    *sql << "UPDATE task_watchdog SET instance_id = :instance_id, device_euid = :device_euid, module_id = :module_id,"
            "operator = :operator, value = :value, notification_text = :notification_text)",
            soci::use(instance_id, "instance_id"), soci::use(conf.device_euid, "device_euid"),
            soci::use(conf.module_id, "module_id"), soci::use(conf.comp_operator, "operator"),
            soci::use(conf.value, "value"), soci::use(conf.notification_text, "notification_text");
}
*/
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