/* 
 * File:   FireHazardManager.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 1. May 2016
 */

#include "FireHazardManager.h"

#include <map>
#include <mutex>
#include <string>

#include <soci.h>

#include "../../../src/DatabaseInterface.h"
#include "../../../src/Logger.h"
#include "FireHazardInstance.h"

extern "C" {
    std::shared_ptr<TaskManager> createTaskManager()
    {
        return std::make_shared<FireHazardManager>();
    }
}

FireHazardManager::FireHazardManager() {
}

FireHazardManager::~FireHazardManager() {
}

void FireHazardManager::createConfiguration(long instance_id, ConfigurationMap config)
{
    // Parse received configuration.
    FireHazardConfig parsed_config = parseConfiguration(instance_id, config);

    // Store configuration to database. This also checks correct configuration data.
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    *sql << "INSERT INTO task_fire_hazard(instance_id, value, gateway_id, device_euid, module_id, a_gateway_id, a_device_euid, a_module_id) "
            "VALUES(:instance_id, :value, :gateway_id, :device_euid, :module_id, :a_gateway_id, :a_device_euid, :a_module_id)",
            soci::use(instance_id, "instance_id"),
            soci::use(parsed_config.value, "value"),
            soci::use(parsed_config.gateway_id, "gateway_id"),
            soci::use(parsed_config.device_euid, "device_euid"),
            soci::use(parsed_config.module_id, "module_id"),
            soci::use(parsed_config.a_gateway_id, "a_gateway_id"),
            soci::use(parsed_config.a_device_euid, "a_device_euid"),
            soci::use(parsed_config.a_module_id, "a_module_id");
    
    // Create new instance of AliveCheck in system.
    std::lock_guard<std::mutex> lock(m_task_instances_mx);
    m_task_instances.emplace(instance_id, std::make_shared<FireHazardInstance>(instance_id, shared_from_this(), parsed_config.device_euid));
    
    logger.LOGFILE("fire_hazard", "INFO") << "New instance of FireHazard was created: instance_id: "
            << instance_id << std::endl;
}

void FireHazardManager::changeConfiguration(ChangeMessage change_message)
{
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    soci::transaction tr(*sql);
    
    // If exists get new value.
    auto value_it = findConfigurationItem(false, "value", &(change_message.config));
    if (value_it != change_message.config.end()) {
        
        double value = std::stod(value_it->second);
        *sql << "UPDATE task_fire_hazard SET value = :value WHERE instance_id = :instance_id",
                soci::use(value, "value"),
                soci::use(change_message.instance_id, "instance_id");
    }
    
    long long gateway_id, new_gateway_id, a_gateway_id, new_a_gateway_id;
    long device_euid, new_device_euid, a_device_euid, new_a_device_euid;
    int module_id, new_module_id, a_module_id, new_a_module_id;
    bool changed_gateway_id = false, changed_device_euid = false, changed_module_id = false;
    bool changed_a_gateway_id = false, changed_a_device_euid = false, changed_a_module_id = false;
    
    // If exists get new gateway_id.
    auto gateway_id_it = findConfigurationItem(false, "gateway_id", &(change_message.config));
    if (gateway_id_it != change_message.config.end()) {
        
        gateway_id = std::stol(gateway_id_it->second);
        validateGatewayOwnership(change_message.instance_id, gateway_id);
        changed_gateway_id = true;
    }
    // If exists get new device_euid.
    auto device_euid_it = findConfigurationItem(false, "device_euid", &(change_message.config));
    if (device_euid_it != change_message.config.end()) {
        
        device_euid = std::stol(device_euid_it->second);
        changed_device_euid = true;
    }
    // If exists get new module_id.
    auto module_id_it = findConfigurationItem(false, "module_id", &(change_message.config));
    if (module_id_it != change_message.config.end()) {
        
        module_id = std::stoi(module_id_it->second);
        changed_module_id = true;
    }
    
    // If exists get new gateway_id.
    auto a_gateway_id_it = findConfigurationItem(false, "a_gateway_id", &(change_message.config));
    if (a_gateway_id_it != change_message.config.end()) {
        
        a_gateway_id = std::stol(a_gateway_id_it->second);
        validateGatewayOwnership(change_message.instance_id, a_gateway_id);
        changed_a_gateway_id = true;
    }
    
    // If exists get new device_euid.
    auto a_device_euid_it = findConfigurationItem(false, "a_device_euid", &(change_message.config));
    if (a_device_euid_it != change_message.config.end()) {
        
        a_device_euid = std::stol(a_device_euid_it->second);
        changed_a_device_euid = true;
    }
    // If exists get new module_id.
    auto a_module_id_it = findConfigurationItem(false, "a_module_id", &(change_message.config));
    if (a_module_id_it != change_message.config.end()) {
        
        a_module_id = std::stoi(a_module_id_it->second);
        changed_a_module_id = true;
    }
    // Get old configuration from database.
    *sql << "SELECT gateway_id, device_euid, module_id, a_gateway_id, a_device_euid,"
            "a_module_id FROM task_fire_hazard WHERE instance_id = :instance_id",
            soci::use(change_message.instance_id),
            soci::into(new_gateway_id),
            soci::into(new_device_euid),
            soci::into(new_module_id),
            soci::into(new_a_gateway_id),
            soci::into(new_a_device_euid),
            soci::into(new_a_module_id);
    
    if (changed_gateway_id) {
        new_gateway_id = gateway_id;
    }
    if (changed_device_euid) {
        new_device_euid = device_euid;
    }
    if (changed_module_id) {
        new_module_id = module_id;
    }
    if (changed_a_gateway_id) {
        new_a_gateway_id = a_gateway_id;
    }
    if (changed_a_device_euid) {
        new_a_device_euid = a_device_euid;
    }
    if (changed_a_module_id) {
        new_a_module_id = a_module_id;
    }
    *sql << "UPDATE task_fire_hazard SET gateway_id = :gateway_id, device_euid = :device_euid, module_id = :module_id, "
            "a_gateway_id = :a_gateway_id, a_device_euid = :a_device_euid, a_module_id = :a_module_id WHERE instance_id = :instance_id",
            soci::use(change_message.instance_id, "instance_id"),
            soci::use(new_gateway_id, "gateway_id"),
            soci::use(new_device_euid, "device_euid"),
            soci::use(new_module_id, "module_id"),
            soci::use(new_a_gateway_id, "a_gateway_id"),
            soci::use(new_a_device_euid, "a_device_euid"),
            soci::use(new_a_module_id, "a_module_id");
        
    if (changed_device_euid) {
        // If update of new device_euid to database was successful, reregister instance in DataMessageRegister.
        std::shared_ptr<FireHazardInstance> changed_instance = std::dynamic_pointer_cast<FireHazardInstance>(m_task_instances.find(change_message.instance_id)->second);
        changed_instance->changeRegisteredDeviceEuid(new_device_euid);
    }
    // Store to database.
    tr.commit();
    
    logger.LOGFILE("fire_hazard", "INFO") << "Instance of FireHazard was changed: instance_id: "
            << change_message.instance_id << std::endl;
}

std::map<std::string, std::string> FireHazardManager::getConfiguration(GetConfMessage get_conf_message)
{
    FireHazardConfig fire_hazard_config;
    std::map<std::string, std::string> config_map;
    
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    
    std::string type_str;
    *sql << "SELECT value, gateway_id, device_euid, module_id, a_gateway_id, a_device_euid, a_module_id " // It's crucial to have space at the end of string.
            "FROM task_fire_hazard WHERE instance_id = :instance_id",
            soci::use(get_conf_message.instance_id, "instance_id"),
            soci::into(type_str),
            soci::into(fire_hazard_config.value),
            soci::into(fire_hazard_config.gateway_id),
            soci::into(fire_hazard_config.device_euid),
            soci::into(fire_hazard_config.module_id),
            soci::into(fire_hazard_config.a_gateway_id),
            soci::into(fire_hazard_config.a_device_euid),
            soci::into(fire_hazard_config.a_module_id);
    
    config_map["value"] = std::to_string(fire_hazard_config.value);
    config_map["gateway_id"] = std::to_string(fire_hazard_config.gateway_id);
    config_map["device_euid"] = std::to_string(fire_hazard_config.device_euid);
    config_map["module_id"] = std::to_string(fire_hazard_config.module_id);
    config_map["a_gateway_id"] = std::to_string(fire_hazard_config.a_gateway_id);
    config_map["a_device_euid"] = std::to_string(fire_hazard_config.a_device_euid);
    config_map["a_module_id"] = std::to_string(fire_hazard_config.a_module_id);
    
    return config_map;
}

void FireHazardManager::reloadInstances(unsigned int task_id)
{
    long instance_id;
    long device_euid;
    
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    
    soci::rowset<soci::row> rows = (sql->prepare << "SELECT instance_id FROM instance WHERE task_id = :task_id",
                                    soci::use(task_id, "task_id"));

    std::lock_guard<std::mutex> lock(m_task_instances_mx);
    for (soci::rowset<soci::row>::const_iterator it = rows.begin(); it != rows.end(); ++it) {   
        
        soci::row const& row = *it;
        instance_id = row.get<int>(0);
        
        *sql << "SELECT device_euid FROM task_fire_hazard WHERE instance_id = :instance_id",
                soci::use(instance_id, "instance_id"),
                soci::into(device_euid);
        
        m_task_instances.emplace(instance_id, std::make_shared<FireHazardInstance>(instance_id, shared_from_this(), device_euid));
        
        logger.LOGFILE("fire_hazard", "INFO") << "Instance of FireHazard was reloaded: instance_id: "
             << instance_id << std::endl;
    }
}

FireHazardConfig FireHazardManager::parseConfiguration(long instance_id, ConfigurationMap configuration)
{
    FireHazardConfig parsed_config;
    
    // Get item value.
    auto value_it = findConfigurationItem(true, "value", &configuration);
    parsed_config.value = std::stod(value_it->second);
    
    // Get item gateway_id.
    auto gateway_id_it = findConfigurationItem(true, "gateway_id", &configuration);
    parsed_config.gateway_id = std::stoll(gateway_id_it->second);
    validateGatewayOwnership(instance_id, parsed_config.gateway_id);
    
    // Get item device_euid.
    auto device_euid_it = findConfigurationItem(true, "device_euid", &configuration);
    parsed_config.device_euid = std::stol(device_euid_it->second);

    // Get item module_id.
    auto module_id_it = findConfigurationItem(true, "module_id", &configuration);
    parsed_config.module_id = std::stoi(module_id_it->second);

    auto a_gateway_id_it = findConfigurationItem(true, "a_gateway_id", &configuration);
    parsed_config.a_gateway_id = std::stoll(a_gateway_id_it->second);
    validateGatewayOwnership(instance_id, parsed_config.a_gateway_id);
    
    // Get item a_device_euid.
    auto a_device_euid_it = findConfigurationItem(true, "a_device_euid", &configuration);
    parsed_config.a_device_euid = std::stol(a_device_euid_it->second);
        
    // Get item a_module_id.
    auto a_module_id_it = findConfigurationItem(true, "a_module_id", &configuration);
    parsed_config.a_module_id = std::stoi(a_module_id_it->second);
    
    return parsed_config;
}

ConfigurationMap::iterator FireHazardManager::findConfigurationItem(bool required,
                                                                  std::string item_name,
                                                                  ConfigurationMap* configuration)
{
    auto iterator = configuration->find(item_name);
    if (iterator == configuration->end()) {
        
        if (required) {
            // If not present but required throw error.
            logger.LOGFILE("fire_hazard", "ERROR") << "Received configuration doesn't "
                "have item required: " << item_name << std::endl;
            
            throw std::runtime_error(std::string("Could not parse received configuration.") + item_name);
        }
    }
    // If present return iterator to it (or iterator will be end of configuration map).
    return iterator;
}

void FireHazardManager::validateGatewayOwnership(long instance_id, long long gateway_id)
{
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    // Get ID of user owning instance of FireHazard.
    long user_id;
    *sql << "SELECT user_id FROM instance WHERE instance_id = :instance_id",
            soci::use(instance_id, "instance_id"),
            soci::into(user_id);
    
    short owns; 
    *sql << "SELECT exists(SELECT 1 FROM user_gateway WHERE user_id = :user_id AND gateway_id = :gateway_id);",
            soci::use(user_id, "user_id"),
            soci::use(gateway_id, "gateway_id"),
            soci::into(owns);
                
    if (!owns) {
        logger.LOGFILE("fire_hazard", "ERROR") <<  "User with user_id: " << user_id << 
                " tried to operate with gateway he doesn't own: " << gateway_id << ".";
        throw std::runtime_error("Could not process received configuration.");
    }
}
