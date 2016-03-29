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
#include <string>

#include <soci.h>
#include "../../../src/DatabaseInterface.h"

#include "WatchdogManager.h"

WatchdogInstance::WatchdogInstance(unsigned int instance_id, unsigned long long device_euid):
    TriggerTaskInstance(instance_id), m_received_data_once(false)
{
    registerDataMessage(device_euid);
}

WatchdogInstance::~WatchdogInstance()
{
}

void WatchdogInstance::run(DataMessage data_message)
{
    if (!m_received_data_once) {
        // This instance haven't received any data messege before.
        
        SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
        *sql << "SELECT module_id FROM task_watchdog WHERE instance_id = :instance_id",
                soci::into(m_last_received_value), soci::use(m_instance_id, "instance_id");
        
        m_received_data_once = true;
        
        std::cout << "||||||||||| WATCHDOG |||||||||||" << std::endl;
        std::cout << " " << std::endl;
    }
    else {
        WatchdogConfig config;
        
        // Get configuration of this instance from database.
        SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
        *sql << "SELECT device_euid, module_id, operator, value, notification_text FROM task_watchdog WHERE instance_id = :instance_id",
                soci::into(config.device_euid), soci::into(config.module_id), soci::into(config.comp_operator),
                soci::into(config.value), soci::into(config.notification_text),
                soci::use(m_instance_id, "instance_id");

        std::cout << "WATCHDOG HAS RISEN! Device_euid: " << config.device_euid << " | module_id: " << config.module_id << " | received data message euid: " << data_message.device_euid << std::endl;

        /*
        for (auto module : data_message.modules) {

        }
        */
    }
    
    
}
