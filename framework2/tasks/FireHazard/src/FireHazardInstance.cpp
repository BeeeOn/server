/* 
 * File:   FireHazardInstance.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 4. May 2016
 */

#include "FireHazardInstance.h"

#include <iostream>
#include <chrono>
#include <stdexcept>
#include <string>

#include <soci.h>

#include "../../../src/DatabaseInterface.h"
#include "../../../src/GatewayInterface.h"
#include "../../../src/Logger.h"

FireHazardInstance::FireHazardInstance(long instance_id,
                                       std::weak_ptr<TaskManager> owning_manager,
                                       long device_euid):
    TaskInstance(instance_id, owning_manager),    
    CombinedTaskInstance(instance_id, owning_manager),
    m_blink_sequence(0),
    m_sequence_running(false)
{
    registerToReceiveDataFromDevice(device_euid);
}


FireHazardInstance::~FireHazardInstance() {
}

void FireHazardInstance::run(DataMessage data_message)
{
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    
    int module_id;
    double value;
    *sql << "SELECT value, module_id FROM task_fire_hazard WHERE instance_id = :instance_id",
            soci::use(m_instance_id, "instance_id"),
            soci::into(value),
            soci::into(module_id);
    
    double received_value = getModuleValue(module_id, data_message);
    
    if (received_value > value) {
        
        if (!m_sequence_running) {
            // If received value is above guarded value, and sequence isn't
            // already sending, start sending blink sequence.
            m_sequence_running = true;
            planActivationNow(); 
        }
    }
    else {
        if (m_sequence_running) {
            // If received value is below guarded value, stop sending blink sequence.
            deleteFromCalendar();
            // Restart sequence of blink signal.
            m_blink_sequence = 0;
            // Sequence was stopped.
            m_sequence_running = false;
            // Switch off actuator. In the case that sequence was stopped when it was on.
            ActuatorInfo actuator_info = getActuatorInfo();
            GatewayInterface gi;
            //gi.sendSetState(actuator_info.a_gateway_id, actuator_info.a_device_euid,
                                //actuator_info.a_module_id, 0);
            logger.LOGFILE("fire_hazard", "INFO") << "Instance FireHazard: " << m_instance_id << " switched OFF an actuator: [gateway_id: "
                << actuator_info.a_gateway_id << ", device_euid: " << actuator_info.a_device_euid << ", module_id: "
                << actuator_info.a_module_id << "]" << std::endl;
        }
    }
}

void FireHazardInstance::run(std::chrono::system_clock::time_point activation_time)
{
    // Get needed information about actuator from configuration.
    ActuatorInfo actuator_info = getActuatorInfo();

    GatewayInterface gi;
    if (m_blink_sequence == 0) {
        
        // Set actuator (power switch to ON).
        gi.sendSetState(actuator_info.a_gateway_id, actuator_info.a_device_euid,
                        actuator_info.a_module_id, 1);
        logger.LOGFILE("fire_hazard", "INFO") << "Instance FireHazard: " << m_instance_id << " switched ON an actuator: [gateway_id: "
                << actuator_info.a_gateway_id << ", device_euid: " << actuator_info.a_device_euid << ", module_id: "
                << actuator_info.a_module_id << "]" << std::endl;
        planActivationAfterSeconds(2);
    }
    else {
        // Set actuator (power switch to OFF).
        gi.sendSetState(actuator_info.a_gateway_id, actuator_info.a_device_euid,
                        actuator_info.a_module_id, 0);
        logger.LOGFILE("fire_hazard", "INFO") << "Instance FireHazard: " << m_instance_id << " switched OFF an actuator: [gateway_id: "
                << actuator_info.a_gateway_id << ", device_euid: " << actuator_info.a_device_euid << ", module_id: "
                << actuator_info.a_module_id << "]" << std::endl;
        planActivationAfterSeconds(1);
        
    }
    
    // Set next state of blink sequence.
    if (m_blink_sequence == 1) {
        m_blink_sequence = 0;
    }
    else {
        m_blink_sequence = 1;
    }
}

double FireHazardInstance::getModuleValue(int module_id, DataMessage data_message)
{
    auto module_it = data_message.modules.find(module_id);
    if (module_it == data_message.modules.end()) {
            
        logger.LOGFILE("fire_hazard", "ERROR") << "Wanted module_id: " << module_id <<" was not found in"
                " received message: instance_id" << m_instance_id << std::endl; 
        throw std::runtime_error("Run of FireHazard instance was not successful.");      
    }
    // Return received value.
    return module_it->second.second;
}

ActuatorInfo FireHazardInstance::getActuatorInfo()
{
    ActuatorInfo actuator_info;
    
    soci::indicator ind_a_device_euid, ind_a_module_id, ind_a_gateway_id;
    
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    // Get actuator device_euid and module_id from database.
    *sql << "SELECT a_device_euid, a_module_id "
            "FROM task_fire_hazard WHERE instance_id = :instance_id",
             soci::use(m_instance_id, "instance_id"),
             soci::into(actuator_info.a_device_euid, ind_a_device_euid),
             soci::into(actuator_info.a_module_id, ind_a_module_id);
        
    if (ind_a_device_euid != soci::i_ok || ind_a_module_id != soci::i_ok) {
            
        logger.LOGFILE("fire_hazard", "ERROR") << "Wanted actuator device_euid and"
                "module id doesn't exist in database! " << std::endl; 
        throw std::runtime_error("Run of FireHazard instance was not successful.");   
    }
    else {
        // Get ID of gateway on which actuator is from database. If device exists, gateway should too.
        *sql << "SELECT gateway_id FROM device WHERE device_euid = :device_euid",
                 soci::use(actuator_info.a_device_euid, "device_euid"),
                 soci::into(actuator_info.a_gateway_id, ind_a_gateway_id);
    }
    return actuator_info;
}
