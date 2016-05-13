/* 
 * File:   WatchdogManager.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 29. March 2016
 */

#include "WatchdogManager.h"

#include <map>
#include <mutex>
#include <string>

#include <soci.h>

#include "../../../src/DatabaseInterface.h"
#include "../../../src/Logger.h"

extern "C" {
    std::shared_ptr<TaskManager> createTaskManager()
    {
        return std::make_shared<WatchdogManager>();
    }
}

WatchdogManager::WatchdogManager()
{
}

WatchdogManager::~WatchdogManager()
{
}

void WatchdogManager::createConfiguration(long instance_id, ConfigurationMap config)
{
    // Parse received configuration.
    WatchdogConfig parsed_config = parseCreateConfiguration(instance_id, config);
    
    // Store configuration to database. This also checks correct configuration data.
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    
    *sql << "INSERT INTO task_watchdog(instance_id, type, gateway_id, device_euid, module_id, comp_operator, "
            "value) VALUES(:instance_id, :type, :gateway_id, :device_euid, :module_id, :comp_operator, :value)",
            soci::use(instance_id, "instance_id"),
            soci::use(convertWatchdogTypeToString(parsed_config.type), "type"),
            soci::use(parsed_config.gateway_id, "gateway_id"),
            soci::use(parsed_config.device_euid, "device_euid"),
            soci::use(parsed_config.module_id, "module_id"),
            soci::use(parsed_config.comp_operator, "comp_operator"),
            soci::use(parsed_config.value, "value");
    
    if (parsed_config.type == WatchdogType::NOTIF || parsed_config.type == WatchdogType::BOTH) {
        
        *sql << "UPDATE task_watchdog SET notification = :notification WHERE instance_id = :instance_id",
                soci::use(instance_id, "instance_id"),
                soci::use(parsed_config.notification, "notification");
                
    }
    if (parsed_config.type == WatchdogType::SWITCH || parsed_config.type == WatchdogType::BOTH) {
    
        *sql << "UPDATE task_watchdog SET a_gateway_id = :a_gateway_id, a_device_euid = :a_device_euid, "
                "a_module_id = :a_module_id, a_value = :a_value WHERE instance_id = :instance_id",
                soci::use(instance_id, "instance_id"),
                soci::use(parsed_config.a_gateway_id, "a_gateway_id"),
                soci::use(parsed_config.a_device_euid, "a_device_euid"),
                soci::use(parsed_config.a_module_id, "a_module_id"),
                soci::use(parsed_config.a_value, "a_value");
    }
    std::lock_guard<std::mutex> lock(m_task_instances_mx);
    m_task_instances.emplace(instance_id, std::make_shared<WatchdogInstance>(instance_id, shared_from_this(), parsed_config.device_euid));
    
    logger.LOGFILE("watchdog", "INFO") << "New instance of Watchdog was created: instance_id: "
            << instance_id << std::endl;
}

void WatchdogManager::changeConfiguration(ChangeMessage change_message)
{
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    
    // Get old watchdog type from database.
    std::string type_str;
    *sql << "SELECT type FROM task_watchdog WHERE instance_id = :instance_id",
            soci::into(type_str), soci::use(change_message.instance_id);
    WatchdogType old_type = convertStringToWatchdogType(type_str);
    WatchdogType new_type = old_type;

    soci::transaction tr(*sql);
    
    // If exists get new type.
    auto type_it = findConfigurationItem(false, "type", &(change_message.config));
    if (type_it != change_message.config.end()) {
        
        new_type = convertStringToWatchdogType(type_it->second);
        std::string new_type_str = type_it->second;
        *sql << "UPDATE task_watchdog SET type = :type WHERE instance_id = :instance_id",
                soci::use(new_type_str, "type"),
                soci::use(change_message.instance_id, "instance_id");
    }

    long long gateway_id;
    long device_euid;
    int module_id;
    bool changed_gateway_id = false, changed_device_euid = false, changed_module_id = false;
    
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
    // Gateway, device and module must be updated in the single statement so database checks 
    // foreign keys of new and old configuration at the same time. I know it looks horrible, but it works good.
    if (changed_gateway_id || changed_device_euid || changed_module_id) {
        long long new_gateway_id;
        long new_device_euid;
        int new_module_id;
        // Get old configuration from database.
        *sql << "SELECT gateway_id, device_euid, module_id FROM task_watchdog WHERE instance_id = :instance_id",
                soci::use(change_message.instance_id),
                soci::into(new_gateway_id),
                soci::into(new_device_euid),
                soci::into(new_module_id);
        if (changed_gateway_id) {
            new_gateway_id = gateway_id;
        }
        if (changed_device_euid) {
            new_device_euid = device_euid;
        }
        if (changed_module_id) {
            new_module_id = module_id;
        }
        *sql << "UPDATE task_watchdog SET gateway_id = :gateway_id, device_euid = :device_euid, "
                "module_id = :module_id WHERE instance_id = :instance_id",
                soci::use(change_message.instance_id, "instance_id"),
                soci::use(new_gateway_id, "gateway_id"),
                soci::use(new_device_euid, "device_euid"),
                soci::use(new_module_id, "module_id");
        
        if (changed_device_euid) {
            // If update of new device_euid to database was successful, reregister instance in DataMessageRegister.
            std::shared_ptr<WatchdogInstance> changed_instance = std::dynamic_pointer_cast<WatchdogInstance>(m_task_instances.find(change_message.instance_id)->second);
            changed_instance->changeRegisteredDeviceEuid(new_device_euid);
        }
    }

    auto comp_operator_it = findConfigurationItem(false, "comp_operator", &(change_message.config));
    if (comp_operator_it != change_message.config.end()) {
        
        std::string comp_operator = comp_operator_it->second;
        validateCompOperator(comp_operator);
        
        *sql << "UPDATE task_watchdog SET comp_operator = :comp_operator WHERE instance_id = :instance_id",
            soci::use(comp_operator, "comp_operator"),
            soci::use(change_message.instance_id, "instance_id");
    }
    
    auto value_it = findConfigurationItem(false, "value", &(change_message.config));
    if (value_it != change_message.config.end()) {
        
        double value = std::stod(value_it->second);
        *sql << "UPDATE task_watchdog SET value = :value WHERE instance_id = :instance_id",
                soci::use(value, "value"),
                soci::use(change_message.instance_id, "instance_id");
    }
    
    // If type didn't change, change configuration which is found.
    // Ignore configuration of other types.
    bool type_changed;
    if (old_type == new_type) {
        type_changed = false;
    }
    else {
        type_changed = true;
    }
    
    if (new_type == WatchdogType::NOTIF || new_type == WatchdogType::BOTH) {
    
        // If type changed to notif or both, this item must be present.
        auto notification_it = findConfigurationItem(type_changed, "notification", &(change_message.config));
        if (notification_it != change_message.config.end()) {
            
            std::string notification = notification_it->second;
            *sql << "UPDATE task_watchdog SET notification = :notification WHERE instance_id = :instance_id",
                    soci::use(notification, "notification"),
                    soci::use(change_message.instance_id, "instance_id");
        }
    }
    if (new_type == WatchdogType::SWITCH || new_type == WatchdogType::BOTH) {

        long long a_gateway_id;
        long a_device_euid;
        int a_module_id;
        bool changed_a_gateway_id = false, changed_a_device_euid = false, changed_a_module_id = false;
        // If type changed to switch or both, this item must be present.
        // If exists get new gateway_id.
        auto a_gateway_id_it = findConfigurationItem(type_changed, "a_gateway_id", &(change_message.config));
        if (a_gateway_id_it != change_message.config.end()) {

            a_gateway_id = std::stol(a_gateway_id_it->second);
            validateGatewayOwnership(change_message.instance_id, a_gateway_id);
            changed_a_gateway_id = true;
        }
        // a_device_euid;
        auto a_device_euid_it = findConfigurationItem(type_changed, "a_device_euid", &(change_message.config));
        if (a_device_euid_it != change_message.config.end()) {

            a_device_euid = std::stol(a_device_euid_it->second);
            changed_a_device_euid = true;
            /*
            *sql << "UPDATE task_watchdog SET a_device_euid = :a_device_euid WHERE instance_id = :instance_id",
                    soci::use(a_device_euid, "a_device_euid"),
                    soci::use(change_message.instance_id, "instance_id");
            */
        }
        // If type changed to switch or both, this item must be present.
        auto a_module_id_it = findConfigurationItem(type_changed, "a_module_id", &(change_message.config));
        if (a_module_id_it != change_message.config.end()) {

            a_module_id = std::stoi(a_module_id_it->second);
            changed_a_module_id = true;
            /*
            *sql << "UPDATE task_watchdog SET a_module_id = :a_module_id WHERE instance_id = :instance_id",
                    soci::use(a_module_id, "a_module_id"),
                    soci::use(change_message.instance_id, "instance_id");
            */
        }
        if (changed_a_gateway_id || changed_a_device_euid || changed_a_module_id) {
            long long new_a_gateway_id;
            long new_a_device_euid;
            int new_a_module_id;
            // Get old configuration from database.
            *sql << "SELECT a_gateway_id, a_device_euid, a_module_id FROM task_watchdog WHERE instance_id = :instance_id",
                    soci::use(change_message.instance_id),
                    soci::into(new_a_gateway_id),
                    soci::into(new_a_device_euid),
                    soci::into(new_a_module_id);
            if (changed_a_gateway_id) {
                new_a_gateway_id = a_gateway_id;
            }
            if (changed_device_euid) {
                new_a_device_euid = a_device_euid;
            }
            if (changed_module_id) {
                new_a_module_id = module_id;
            }
            *sql << "UPDATE task_watchdog SET a_gateway_id = :a_gateway_id, a_device_euid = :a_device_euid, "
                    "a_module_id = :a_module_id WHERE instance_id = :instance_id",
                    soci::use(change_message.instance_id, "instance_id"),
                    soci::use(new_a_gateway_id, "a_gateway_id"),
                    soci::use(new_a_device_euid, "a_device_euid"),
                    soci::use(new_a_module_id, "a_module_id");
        }
        // If type changed to switch or both, this item must be present.
        auto a_value_it = findConfigurationItem(type_changed, "a_value", &(change_message.config));
        if (a_value_it != change_message.config.end()) {

            int a_value = std::stoi(a_value_it->second);
            *sql << "UPDATE task_watchdog SET a_value = :a_value WHERE instance_id = :instance_id",
                    soci::use(a_value, "a_value"),
                    soci::use(change_message.instance_id, "instance_id");
        }
    }
    // Store to database.
    tr.commit();
    
    logger.LOGFILE("watchdog", "INFO") << "Instance of Watchdog was changed: instance_id: "
            << change_message.instance_id << std::endl;
}

std::map<std::string, std::string> WatchdogManager::getConfiguration(GetConfMessage get_conf_message)
{
    WatchdogConfig watchdog_config;
    std::map<std::string, std::string> config_map;
    
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    
    std::string type_str;
    *sql << "SELECT type, gateway_id, device_euid, module_id, comp_operator, value " // It's crucial to have space at the end of string.
            "FROM task_watchdog WHERE instance_id = :instance_id",
            soci::use(get_conf_message.instance_id, "instance_id"),
            soci::into(type_str),
            soci::into(watchdog_config.gateway_id),
            soci::into(watchdog_config.device_euid),
            soci::into(watchdog_config.module_id),
            soci::into(watchdog_config.comp_operator),
            soci::into(watchdog_config.value);
    
    config_map["type"] = type_str;
    config_map["gateway_id"] = std::to_string(watchdog_config.gateway_id);
    config_map["device_euid"] = std::to_string(watchdog_config.device_euid);
    config_map["module_id"] = std::to_string(watchdog_config.module_id);
    config_map["comp_operator"] = watchdog_config.comp_operator;
    config_map["value"] = std::to_string(watchdog_config.value);

    WatchdogType type = convertStringToWatchdogType(type_str); 
            
    if (type == WatchdogType::NOTIF || type == WatchdogType::BOTH) {
        
        *sql << "SELECT notification FROM task_watchdog WHERE instance_id = :instance_id",
            soci::use(get_conf_message.instance_id, "instance_id"),
            soci::into(watchdog_config.notification);
        
        config_map["notification"] = watchdog_config.notification;
    }
    if (type == WatchdogType::SWITCH || type == WatchdogType::BOTH) {
        
        *sql << "SELECT a_gateway_id, a_device_euid, a_module_id, a_value " // It's crucial to have space at the end of string.
            "FROM task_watchdog WHERE instance_id = :instance_id",
            soci::use(get_conf_message.instance_id, "instance_id"),
            soci::into(watchdog_config.a_gateway_id),
            soci::into(watchdog_config.a_device_euid),
            soci::into(watchdog_config.a_module_id),
            soci::into(watchdog_config.a_value);
        
        config_map["a_gateway_id"] = std::to_string(watchdog_config.a_gateway_id);
        config_map["a_device_euid"] = std::to_string(watchdog_config.a_device_euid);
        config_map["a_module_id"] = std::to_string(watchdog_config.a_module_id);
        config_map["a_value"] = std::to_string(watchdog_config.a_value);
    }

    logger.LOGFILE("watchdog", "INFO") << "Configuration of instance of Watchdog was requested: instance_id: "
         << get_conf_message.instance_id << std::endl;
    
    return config_map;
}

void WatchdogManager::reloadInstances(unsigned int task_id)
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
        
        *sql << "SELECT device_euid FROM task_watchdog WHERE instance_id = :instance_id",
                soci::use(instance_id, "instance_id"),
                soci::into(device_euid);
        
        m_task_instances.emplace(instance_id, std::make_shared<WatchdogInstance>(instance_id, shared_from_this(), device_euid));
        
        logger.LOGFILE("watchdog", "INFO") << "Instance of Watchdog was reloaded: instance_id: "
             << instance_id << std::endl;
    }
}

WatchdogConfig WatchdogManager::parseCreateConfiguration(long instance_id, ConfigurationMap configuration)
{
    WatchdogConfig parsed_config;
    
    // Get type of Watchdog.
    auto type_it = findConfigurationItem(true, "type", &configuration);
    parsed_config.type = convertStringToWatchdogType(type_it->second);
    
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
    //validateModuleExistence(parsed_config.device_euid, parsed_config.module_id);
    
    // Get item comp_operator.
    auto comp_operator_it = findConfigurationItem(true, "comp_operator", &configuration);
    // Check if comp_operator is GT or LT.
    validateCompOperator(comp_operator_it->second);
    parsed_config.comp_operator = comp_operator_it->second;
    
    // Get item value.
    auto value_it = findConfigurationItem(true, "value", &configuration);
    parsed_config.value = std::stod(value_it->second);
    
    // Now parsing differs based on type.
    if (parsed_config.type == WatchdogType::NOTIF || parsed_config.type == WatchdogType::BOTH) {
        
        // If the type is NOTIF or BOTH, get notification.
        auto notification_it = findConfigurationItem(true, "notification", &configuration);
        parsed_config.notification = notification_it->second;
    }
    
    if (parsed_config.type == WatchdogType::SWITCH || parsed_config.type == WatchdogType::BOTH) {
        
        // If the type is SWITCH or BOTH, parse info about actuator.
        // Get item a_gateway_id.
        auto a_gateway_id_it = findConfigurationItem(true, "a_gateway_id", &configuration);
        parsed_config.a_gateway_id = std::stoll(a_gateway_id_it->second);
        validateGatewayOwnership(instance_id, parsed_config.a_gateway_id);
        
        // Get item a_device_euid.
        auto a_device_euid_it = findConfigurationItem(true, "a_device_euid", &configuration);
        parsed_config.a_device_euid = std::stol(a_device_euid_it->second);
        
        
        // Get item a_module_id.
        auto a_module_id_it = findConfigurationItem(true, "a_module_id", &configuration);
        parsed_config.a_module_id = std::stoi(a_module_id_it->second);
        //validateModuleExistence(parsed_config.a_device_euid, parsed_config.a_module_id);
        
        // Get item a_value.
        auto a_value_it = findConfigurationItem(true, "a_value", &configuration);
        parsed_config.a_value = std::stoi(a_value_it->second);
    }

    return parsed_config;
}

ConfigurationMap::iterator WatchdogManager::findConfigurationItem(bool required,
                                                                  std::string item_name,
                                                                  ConfigurationMap* configuration)
{
    auto iterator = configuration->find(item_name);
    if (iterator == configuration->end()) {
        
        if (required) {
            // If not present but required throw error.
            logger.LOGFILE("watchdog", "ERROR") << "Received configuration doesn't "
                "have item required: " << item_name << std::endl;
            
            throw std::runtime_error(std::string("Could not parse received configuration.") + item_name);
        }
    }
    // If present return iterator to it (or iterator will be end of configuration map).
    return iterator;
}

WatchdogType WatchdogManager::convertStringToWatchdogType(std::string type_str)
{
    WatchdogType type;
    // Decide which type of Watchdog it is.
    if (type_str == "NOTIF") {
        type = WatchdogType::NOTIF;
    }
    else if (type_str == "SWITCH") {
        type = WatchdogType::SWITCH;
    }
    else if (type_str == "BOTH") {
        type = WatchdogType::BOTH;
    }
    else {
        // If it's not recognized, throw error.
        logger.LOGFILE("watchdog", "ERROR") << "Type in received configuration "
                       "was not recognized." << std::endl;
        throw std::runtime_error("Could not parse received configuration.");
    }
    return type;
}

std::string WatchdogManager::convertWatchdogTypeToString(WatchdogType type)
{
    std::string type_str;
    switch (type) {
        case (WatchdogType::NOTIF):
            type_str = "NOTIF";
            break;
        case (WatchdogType::SWITCH):
            type_str = "SWITCH";
            break;
        default:
            type_str = "BOTH";
            break;
    }
    return type_str;
}

void WatchdogManager::validateCompOperator(std::string comp_operator)
{
    
    if (comp_operator != "LT" && comp_operator != "GT") {

        logger.LOGFILE("watchdog", "ERROR") << "comp_operator of received configuration "
                "is not GT or LT." << std::endl;
        throw std::runtime_error("Could not parse received configuration.");
    }
}

void WatchdogManager::validateGatewayOwnership(long instance_id, long long gateway_id)
{
    SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
    // Get ID of user owning instance of Watchdog.
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
        logger.LOGFILE("watchdog", "ERROR") <<  "User with user_id: " << user_id << 
                " tried to operate with gateway he doesn't own: " << gateway_id << ".";
        throw std::runtime_error("Could not process received configuration.");
    }
}
