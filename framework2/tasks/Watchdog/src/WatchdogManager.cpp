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
            
            /*"INSERT INTO task_alive_check(instance_id, notifications, gateway_id) VALUES(:instance_id, :notifications, :gateway_id)",
            
            
            soci::use(notifications, "notifications");
    */
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
