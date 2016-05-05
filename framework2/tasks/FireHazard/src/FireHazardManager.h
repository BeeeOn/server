/* 
 * File:   FireHazardManager.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 4. May 2016
 */

#ifndef FIREHAZARDMANAGER_H
#define FIREHAZARDMANAGER_H

#include <iostream>
#include <map>
#include <string>

#include "../../../src/DatabaseInterface.h"
#include "../../../src/TaskManager.h"

#include "FireHazardInstance.h"

typedef std::map<std::string, std::string> ConfigurationMap;


struct FireHazardConfig {
    double value;
    long device_euid;
    int module_id;
    long a_device_euid;
    int a_module_id;
};

class FireHazardManager: public TaskManager
{
public:
    /**
     * Constructor of class FireHazardManager.
     */
    FireHazardManager();
    /**
     * Destructor of class FireHazardManager.
     */
    virtual ~FireHazardManager();
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
    
    FireHazardConfig parseConfiguration(long instance_id, ConfigurationMap configuration);
    
    ConfigurationMap::iterator findConfigurationItem(bool required, std::string item_name, ConfigurationMap* configuration);
    
    /**
     * Checks in database if owner of instance also have access to gateway on which device is.
     */
    void validateDeviceOwnership(long instance_id, long device_euid);
    /**
     * Checks in database if device really contains passed module.
     */
    void validateModuleExistance(long device_euid, int module_id);
};

#endif /* FIREHAZARDMANAGER_H */
