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
    m_sos_sequence(0),
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
            // already sending, start sending SOS sequence.
            m_sequence_running = true;
            planActivationNow(); 
        }
    }
    else {
        // If received value is below guarded value, stop sending SOS sequence.
        deleteFromCalendar();
        // Restart sequence of SOS signal.
        m_sos_sequence = 0;
        // Sequence was stopped.
        m_sequence_running = false;
        // Switch off actuator. In the case that sequence was stopped when it was on.
        ActuatorInfo actuator_info = getActuatorInfo();
        GatewayInterface gi;
        gi.sendSetState(actuator_info.a_gateway_id, actuator_info.a_device_euid,
                            actuator_info.a_module_id, 0);
    }
}

void FireHazardInstance::run(std::chrono::system_clock::time_point activation_time)
{
    // Get needed information about actuator from configuration.
    ActuatorInfo actuator_info = getActuatorInfo();

    GatewayInterface gi;
    switch (m_sos_sequence) {
        case(0):
        case(2):
        case(4):
        case(12):
        case(14):
        case(16):
            gi.sendSetState(actuator_info.a_gateway_id, actuator_info.a_device_euid,
                            actuator_info.a_module_id, 1);
            planActivationAfterSeconds(1);
            break;
        case(1):
        case(3):
        case(5):
        case(7):
        case(9):
        case(11):
        case(13):
        case(15):
            gi.sendSetState(actuator_info.a_gateway_id, actuator_info.a_device_euid,
                            actuator_info.a_module_id, 0);
            planActivationAfterSeconds(1);
            break;
        case(6):
        case(8):
        case(10):
            gi.sendSetState(actuator_info.a_gateway_id, actuator_info.a_device_euid,
                            actuator_info.a_module_id, 1);
            planActivationAfterSeconds(2);
            break;
        case(17):
            gi.sendSetState(actuator_info.a_gateway_id, actuator_info.a_device_euid,
                            actuator_info.a_module_id, 0);
            planActivationAfterSeconds(2);
            break;
    } 
    // Set next state of SOS sequence.
    if (m_sos_sequence == 17) {
        m_sos_sequence = 0;
    }
    else {
        m_sos_sequence++;
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
    long a_device_euid;
    int a_module_id;
    long long a_gateway_id;
    
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    // Get actuator device_euid and module_id from database.
    *sql << "SELECT a_device_euid, a_module_id "
            "FROM task_fire_hazard WHERE instance_id = :instance_id",
             soci::use(m_instance_id, "instance_id"),
             soci::into(a_device_euid, ind_a_device_euid),
             soci::into(a_module_id, ind_a_module_id);
        
    if (ind_a_device_euid != soci::i_ok || ind_a_module_id != soci::i_ok) {
            
        logger.LOGFILE("fire_hazard", "ERROR") << "Wanted actuator device_euid and"
                "module id doesn't exist in database! " << std::endl; 
        throw std::runtime_error("Run of FireHazard instance was not successful.");   
    }
    else {
        // Get ID of gateway on which actuator is from database. If device exists, gateway should too.
        *sql << "SELECT gateway_id FROM device WHERE device_euid = :device_euid",
                 soci::use(a_device_euid, "device_euid"),
                 soci::into(a_gateway_id, ind_a_gateway_id);
    }
    return actuator_info;
}
