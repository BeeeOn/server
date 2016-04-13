/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   WatchdogInstance.cpp
 * Author: mrmaidx
 * 
 * Created on 29. března 2016, 12:52
 */

#include "WatchdogInstance.h"

#include <iostream>
#include <stdexcept>
#include <string>

#include <soci.h>
#include "../../../src/DatabaseInterface.h"

#include "WatchdogManager.h"

WatchdogInstance::WatchdogInstance(unsigned int instance_id, TaskManager* owning_manager, unsigned long long device_euid):
    TriggerTaskInstance(instance_id, owning_manager),
    m_received_data_once(false)
{
    registerDataMessage(device_euid);
}

WatchdogInstance::~WatchdogInstance()
{
}

void WatchdogInstance::run(DataMessage data_message)
{   
    
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    if (!m_received_data_once) {
        // This instance haven't received any data messege before.
        int module_id;
        
        *sql << "SELECT module_id FROM task_watchdog WHERE instance_id = :instance_id",
                soci::into(module_id), soci::use(m_instance_id, "instance_id");
        
        auto module = data_message.modules.find(module_id);
        if (module == data_message.modules.end()) {
            
            std::cerr << "Wanted module_id: " << module_id << " was not found in received message." << std::endl;
            throw std::runtime_error("Run of watchdog was not successful.");      
        }
        else {
            m_last_received_value = module->second.second;
            m_received_data_once = true;
            
            std::cout << "||||||||||| WATCHDOG |||||||||||" << std::endl;
            std::cout << "Instance with ID: " << m_instance_id << " was activated for the first time." << std::endl;
            std::cout << "Storing received value: " << m_last_received_value << std::endl;
            std::cout << "||||||||||||||||||||||||||||||||" << std::endl;
        }
    }
    else {
        WatchdogConfig config;
        
        // Get configuration of this instance from database.
        *sql << "SELECT device_euid, module_id, operator, value, notification_text FROM task_watchdog WHERE instance_id = :instance_id",
                soci::into(config.device_euid), soci::into(config.module_id), soci::into(config.comp_operator),
                soci::into(config.value), soci::into(config.notification_text),
                soci::use(m_instance_id, "instance_id");

        auto module = data_message.modules.find(config.module_id);
        if (module == data_message.modules.end()) {
            std::cerr << "Wanted module_id: " << config.module_id << " was not found in received message." << std::endl;
            throw std::runtime_error("Run of watchdog was not successful.");  
        }
        else {
            double current_value = module->second.second;
            
            std::cout << "||||||||||| WATCHDOG |||||||||||" << std::endl;
            std::cout << "Instance with ID: " << m_instance_id << " was activated." << std::endl;
            
            if (config.comp_operator == "lesser") {
                if ((current_value < m_last_received_value) &&
                    (config.value <= m_last_received_value) &&
                    (config.value >= current_value)) {
                    
                    std::cout << "Operation LESSER than WAS FULFILLED." << std::endl;
                    std::cout << "Sending notification: " << config.notification_text << std::endl;
                }
                else {
                    std::cout << "Operation LESSER than WAS NOT FULFILLED." << std::endl;
                }
                
            }
            else if (config.comp_operator == "greater") {
                if ((current_value > m_last_received_value) &&
                    (config.value <= current_value) &&
                    (config.value >= m_last_received_value)) {
                    
                    std::cout << "Operation GREATER than WAS FULFILLED." << std::endl;
                    std::cout << "Sending notification: " << config.notification_text << std::endl;
                }
                else {
                    std::cout << "Operation GREATER than WAS NOT FULFILLED." << std::endl;
                }
            }
            else {
                throw std::runtime_error("Operator stored in Watchdog configuration was not defined.");
            }
            std::cout << "Configuration value: " << config.value << std::endl;
            std::cout << "Last value: " << m_last_received_value << std::endl;
            std::cout << "Current value: " << current_value << std::endl;
            std::cout << "||||||||||||||||||||||||||||||||" << std::endl;
            
            m_last_received_value = current_value;
        }
        //std::cout << "WATCHDOG HAS RISEN! Device_euid: " << config.device_euid << " | module_id: " << config.module_id << " | received data message euid: " << data_message.device_euid << std::endl;

    } 
}
