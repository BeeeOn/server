/*
 * File:   AliveCheckManager.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 21. January 2016
 */

#ifndef ALIVECHECKMANAGER_H
#define ALIVECHECKMANAGER_H

#include <map>
#include <string>

#include "../../../src/UserMessages.h"
#include "../../../src/TaskManager.h"

typedef std::map<std::string, std::string> ConfigurationMap;

struct AliveCheckConfig {
	short send_notif;
	long long gateway_id;
};

class AliveCheckManager: public TaskManager
{
public:
	/**
	 * Constructor of class AliveCheckManager.
	 */
	AliveCheckManager();
	/**
	 * Destructor of class AliveCheckManager.
	 */
	virtual ~AliveCheckManager();
	/**
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
	 * Parses configuration into AliveCheckConfig object.
	 * @param configuration Map of configuration to parse.
	 * @return Parsed AliveCheckConfig object.
	 */
	AliveCheckConfig parseConfiguration(long instance_id, ConfigurationMap configuration);
	/**
	 * Checks in database if owner of instance also have access to gateway.
	 */
	void validateGatewayOwnership(long instance_id, long long gateway_id);
	/**
	 * Validates if send_notif configuration item is 0 or 1.
	 */
	void validateSendNotif(short send_notif);
};
#endif /* ALIVECHECKMANAGER_H */
