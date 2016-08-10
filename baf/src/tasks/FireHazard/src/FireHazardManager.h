/*
 * File:   FireHazardManager.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 1. May 2016
 */

#ifndef FIREHAZARDMANAGER_H
#define FIREHAZARDMANAGER_H

#include <iostream>
#include <map>
#include <string>

#include "../../../server/DatabaseInterface.h"
#include "../../../core/TaskManager.h"

#include "FireHazardInstance.h"

typedef std::map<std::string, std::string> ConfigurationMap;


struct FireHazardConfig {
	double value;
	long long gateway_id;
	long device_euid;
	int module_id;
	long long a_gateway_id;
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
	 * Changes configuration of instance in database.
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
	FireHazardConfig parseConfiguration(long instance_id, ConfigurationMap configuration);
	/**
	 * Finds by item name if it is present in received configuration, if it's it returns iterator to it.
	 * If item is not present and required is set to true it throws an exception,
	 * otherwise it returns iterator to end of configuration.
	 */
	ConfigurationMap::iterator findConfigurationItem(bool required, std::string item_name, ConfigurationMap* configuration);

	/**
	 * Checks in database if owner of instance has rights to use gateway.
	 */
	void validateGatewayOwnership(long instance_id, long long gateway_id);
};

#endif /* FIREHAZARDMANAGER_H */
