/* 
 * File:   FireHazardManager.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 4. May 2016
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
    *sql << "INSERT INTO task_fire_hazard(instance_id, value, device_euid, module_id, a_device_euid, a_module_id) "
            "VALUES(:instance_id, :value, :device_euid, :module_id, :a_device_euid, :a_module_id)",
            soci::use(instance_id, "instance_id"),
            soci::use(parsed_config.value, "value"),
            soci::use(parsed_config.device_euid, "device_euid"),
            soci::use(parsed_config.module_id, "module_id"),
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
    // If exists get new device_euid.
    long device_euid;
    auto device_euid_it = findConfigurationItem(false, "device_euid", &(change_message.config));
    if (device_euid_it != change_message.config.end()) {
        
        device_euid = std::stol(device_euid_it->second);
        validateDeviceOwnership(change_message.instance_id, device_euid);
        *sql << "UPDATE task_fire_hazard SET device_euid = :device_euid WHERE instance_id = :instance_id",
                soci::use(device_euid, "device_euid"),
                soci::use(change_message.instance_id, "instance_id");
    }
    // If exists get new module_id.
    auto module_id_it = findConfigurationItem(false, "module_id", &(change_message.config));
    if (module_id_it != change_message.config.end()) {
        
        int module_id = std::stoi(module_id_it->second);
        validateModuleExistance(device_euid, module_id);
        *sql << "UPDATE task_fire_hazard SET module_id = :module_id WHERE instance_id = :instance_id",
                soci::use(module_id, "module_id"),
                soci::use(change_message.instance_id, "instance_id");
    }
    // If exists get new a_device_euid.
    long a_device_euid;
    auto a_device_euid_it = findConfigurationItem(false, "a_device_euid", &(change_message.config));
    if (a_device_euid_it != change_message.config.end()) {

        a_device_euid = std::stol(a_device_euid_it->second);
        validateDeviceOwnership(change_message.instance_id, a_device_euid);
        *sql << "UPDATE task_fire_hazard SET a_device_euid = :a_device_euid WHERE instance_id = :instance_id",
                 soci::use(a_device_euid, "a_device_euid"),
                 soci::use(change_message.instance_id, "instance_id");
    }
    // If exists get new a_module_id.
    auto a_module_id_it = findConfigurationItem(false, "a_module_id", &(change_message.config));
    if (a_module_id_it != change_message.config.end()) {

        int a_module_id = std::stoi(a_module_id_it->second);
        validateModuleExistance(a_device_euid, a_module_id);
        *sql << "UPDATE task_fire_hazard SET a_module_id = :a_module_id WHERE instance_id = :instance_id",
                 soci::use(a_module_id, "a_module_id"),
                 soci::use(change_message.instance_id, "instance_id");
    }
    
    // Store to database.
    tr.commit();
    
    logger.LOGFILE("fire_hazard", "INFO") << "Instance of Watchdog was changed: instance_id: "
            << change_message.instance_id << std::endl;
}

std::map<std::string, std::string> FireHazardManager::getConfiguration(GetConfMessage get_conf_message)
{
    FireHazardConfig watchdog_config;
    std::map<std::string, std::string> config_map;
    
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    
    std::string type_str;
    *sql << "SELECT value, device_euid, module_id, a_device_euid, a_module_id " // It's crucial to have space at the end of string.
            "FROM task_fire_hazard WHERE instance_id = :instance_id",
            soci::use(get_conf_message.instance_id, "instance_id"),
            soci::into(type_str),
            soci::into(watchdog_config.value),
            soci::into(watchdog_config.device_euid),
            soci::into(watchdog_config.module_id),
            soci::into(watchdog_config.a_device_euid),
            soci::into(watchdog_config.a_module_id);
    
    config_map["value"] = std::to_string(watchdog_config.value);
    config_map["device_euid"] = std::to_string(watchdog_config.device_euid);
    config_map["module_id"] = std::to_string(watchdog_config.module_id);
    config_map["a_device_euid"] = std::to_string(watchdog_config.a_device_euid);
    config_map["a_module_id"] = std::to_string(watchdog_config.a_module_id);
    
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
        
        logger.LOGFILE("fire_hazard", "INFO") << "Instance of Watchdog was reloaded: instance_id: "
             << instance_id << std::endl;
    }
}

FireHazardConfig FireHazardManager::parseConfiguration(long instance_id, ConfigurationMap configuration)
{
    FireHazardConfig parsed_config;
    
    // Get item value.
    auto value_it = findConfigurationItem(true, "value", &configuration);
    parsed_config.value = std::stod(value_it->second);
    
    // Get item device_euid.
    auto device_euid_it = findConfigurationItem(true, "device_euid", &configuration);
    parsed_config.device_euid = std::stol(device_euid_it->second);
    validateDeviceOwnership(instance_id, parsed_config.device_euid);
    
    // Get item module_id.
    auto module_id_it = findConfigurationItem(true, "module_id", &configuration);
    parsed_config.module_id = std::stoi(module_id_it->second);
    validateModuleExistance(parsed_config.device_euid, parsed_config.module_id);
    
    // Get item a_device_euid.
    auto a_device_euid_it = findConfigurationItem(true, "a_device_euid", &configuration);
    parsed_config.a_device_euid = std::stol(a_device_euid_it->second);
    validateDeviceOwnership(instance_id, parsed_config.a_device_euid);
        
    // Get item a_module_id.
    auto a_module_id_it = findConfigurationItem(true, "a_module_id", &configuration);
    parsed_config.a_module_id = std::stoi(a_module_id_it->second);
    validateModuleExistance(parsed_config.a_device_euid, parsed_config.a_module_id);
    
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

void FireHazardManager::validateDeviceOwnership(long instance_id, long device_euid)
{
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    // Get ID of user owning instance of Watchdog.
    long user_id;
    *sql << "SELECT user_id FROM instance WHERE instance_id = :instance_id",
            soci::use(instance_id, "instance_id"),
            soci::into(user_id);
    
    // Get ID of gateway on which device is.
    long long gateway_id;
    *sql << "SELECT gateway_id FROM device WHERE device_euid = :device_euid",
            soci::use(device_euid, "device_euid"),
            soci::into(gateway_id);
    
    short owns; 
    *sql << "SELECT exists(SELECT 1 FROM user_gateway WHERE user_id = :user_id AND gateway_id = :gateway_id);",
            soci::use(user_id, "user_id"),
            soci::use(gateway_id, "gateway_id"),
            soci::into(owns);
                
    if (!owns) {
        logger.LOGFILE("fire_hazard", "ERROR") <<  "User with user_id: " << user_id << 
                " tried to operate with device which is on gateway user doesn't own: " << gateway_id << ".";
        throw std::runtime_error("Could not process received configuration.");
    }
}

void FireHazardManager::validateModuleExistance(long device_euid, int module_id)
{
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    short owns; 
    *sql << "SELECT exists(SELECT 1 FROM module WHERE device_euid = :device_euid AND module_id = :module_id);",
            soci::use(device_euid, "device_euid"),
            soci::use(module_id, "module_id"),
            soci::into(owns);
                
    if (!owns) {
        logger.LOGFILE("fire_hazard", "ERROR") << "User tried to use module with id: "
                << module_id << " which doesn't exist on device: " << device_euid << ".";
        throw std::runtime_error("Could not process received configuration.");
    }
}
