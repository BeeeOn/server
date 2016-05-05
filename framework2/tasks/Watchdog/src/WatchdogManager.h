/* 
 * File:   WatchdogManager.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 29. March 2016
 */

#ifndef WATCHDOGMANAGER_H
#define WATCHDOGMANAGER_H

#include <iostream>
#include <map>
#include <string>

#include "../../../src/DatabaseInterface.h"
#include "../../../src/TaskManager.h"

#include "WatchdogInstance.h"

typedef std::map<std::string, std::string> ConfigurationMap;

enum class WatchdogType {
    NOTIF, SWITCH, BOTH
};

struct WatchdogConfig {
    WatchdogType type;
    long device_euid;
    int module_id;
    std::string comp_operator;
    double value;
    std::string notification;
    long a_device_euid;
    int a_module_id;
    int a_value;
};

class WatchdogManager: public TaskManager
{
public:
    /**
     * Constructor of class WatchdogManager.
     */
    WatchdogManager();
    /**
     * Destructor of class  WatchdogManager.
     */
    virtual ~WatchdogManager();
    /**
     *
     * Stores configuration of given instance in database.
     * @param instance_id ID of instance which configuration should be stored.
     * @param config Configuration to store.
     */
    void createConfiguration(long instance_id, ConfigurationMap config) override;
    /**
     * Changes configuration of instance.
     * @param change_message Message with info about instance and changed configuration.
     */
    void changeConfiguration(ChangeMessage change_message) override;
    /**
     * Gets configuration of instance from database.
     * @param get_conf_message Message with info about instance.
     * @return Map with instance configuration.
     */
    std::map<std::string, std::string> getConfiguration(GetConfMessage get_conf_message) override;
    /**
     * On start of BAF reloads instances from database.
     * In case when BAF needs to be restarted.
     */
    void reloadInstances(unsigned int task_id) override;

private:
    /**
     * Parses received configuration map.
     * @param instance_id ID of instance which is created.
     * @param configuration Configuration to be parsed.
     * @return Object containing parsed configuration.
     */
    WatchdogConfig parseCreateConfiguration(long instance_id, ConfigurationMap configuration);
    /**
     * Converts std::string to WatchdogType.
     */
    WatchdogType convertStringToWatchdogType(std::string type_str);
    /**
     * Converts WatchdogType to std::string
     */
    std::string convertWatchdogTypeToString(WatchdogType type);
    
    ConfigurationMap::iterator findConfigurationItem(bool required, std::string item_name, ConfigurationMap* configuration);
    /**
     * Checks if passed comparation operator is valid.
     * @param comp_operator Operator to validate.
     */
    void validateCompOperator(std::string comp_operator);
    /**
     * Checks in database if owner of instance also have access to gateway on which device is.
     */
    void validateDeviceOwnership(long instance_id, long device_euid);
    /**
     * Checks in database if device really contains passed module.
     */
    void validateModuleExistance(long device_euid, int module_id);
};

#endif /* WATCHDOGMANAGER_H */
