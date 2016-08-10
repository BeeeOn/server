/*
 * File:   AliveCheckManager.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 21. January 2016
 */

#include "AliveCheckManager.h"

#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <iostream>
#include <stdexcept>

#include <soci.h>

#include "../../../server/DatabaseInterface.h"
#include "../../../Logger.h"

#include "AliveCheckInstance.h"

extern "C" {
	std::shared_ptr<TaskManager> createTaskManager()
	{
		return std::make_shared<AliveCheckManager>();
	}
}

AliveCheckManager::AliveCheckManager()
{
}

AliveCheckManager::~AliveCheckManager()
{
}

void AliveCheckManager::createConfiguration(long instance_id, std::map<std::string, std::string> config)
{
	// Parse received configuration.
	AliveCheckConfig parsed_config = parseConfiguration(instance_id, config);

	// Store configuration to database. This also checks correct configuration data.
	SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
	*sql << "INSERT INTO task_alive_check(instance_id, send_notif, gateway_id) VALUES(:instance_id, :send_notif, :gateway_id)",
			soci::use(instance_id, "instance_id"),
			soci::use(parsed_config.gateway_id, "gateway_id"),
			soci::use(parsed_config.send_notif, "send_notif");

	// Create new instance of AliveCheck in system.
	std::lock_guard<std::mutex> lock(m_task_instances_mx);
	m_task_instances.emplace(instance_id, std::make_shared<AliveCheckInstance>(instance_id, shared_from_this()));

	logger.LOGFILE("alive_check", "INFO") << "New instance of AliveCheck was created: instance_id: "
			<< instance_id << std::endl;
}

void AliveCheckManager::changeConfiguration(ChangeMessage change_message)
{
	// Insert changes to database.
	SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
	soci::transaction tr(*sql);

	// Try to find gateway_id member.
	auto gateway_id_it = change_message.config.find("gateway_id");
	if (gateway_id_it != change_message.config.end()) {

		// Convert string to long long.
		long long gateway_id = std::stoll(gateway_id_it->second);
		validateGatewayOwnership(change_message.instance_id, gateway_id);
		// Insert to database.
		*sql << "UPDATE task_alive_check SET gateway_id = :gateway_id WHERE instance_id = :instance_id",
				soci::use(gateway_id, "gateway_id"),
				soci::use(change_message.instance_id, "instance_id");
	}

	auto send_notif_it = change_message.config.find("send_notif");
	if (send_notif_it != change_message.config.end()) {

		// Converts send_notif to short.
		// send_notif must be 1 for true or 0 for false.
		short send_notif = std::stoi(send_notif_it->second);
		validateSendNotif(send_notif);
		// Insert to database.
		*sql << "UPDATE task_alive_check SET send_notif = :send_notif WHERE instance_id = :instance_id",
				soci::use(send_notif, "send_notif"),
				soci::use(change_message.instance_id, "instance_id");
	}
	tr.commit();

	logger.LOGFILE("alive_check", "INFO") << "Instance of AliveCheck was changed: instance_id: "
			<< change_message.instance_id << std::endl;
}

std::map<std::string, std::string> AliveCheckManager::getConfiguration(GetConfMessage get_conf_message)
{
	AliveCheckConfig alive_config;
	std::map<std::string, std::string> config_map;

	SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
	*sql << "SELECT gateway_id, send_notif FROM task_alive_check WHERE instance_id = :instance_id",
			soci::use(get_conf_message.instance_id, "instance_id"),
			soci::into(alive_config.gateway_id),
			soci::into(alive_config.send_notif);

	config_map["gateway_id"] = std::to_string(alive_config.gateway_id);
	config_map["send_notif"] = std::to_string(alive_config.send_notif);
	return config_map;
}

void AliveCheckManager::reloadInstances(unsigned int task_id)
{
	long instance_id;

	SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();

	soci::rowset<soci::row> rows = (sql->prepare << "SELECT instance_id FROM instance WHERE task_id = :task_id",
									soci::use(task_id, "task_id"));

	std::lock_guard<std::mutex> lock(m_task_instances_mx);
	for (soci::rowset<soci::row>::const_iterator it = rows.begin(); it != rows.end(); ++it) {

		soci::row const& row = *it;
		instance_id = row.get<int>(0);

		// Create instance in system.
		m_task_instances.emplace(instance_id, std::make_shared<AliveCheckInstance>(instance_id, shared_from_this()));

		logger.LOGFILE("alive_check", "INFO") << "Instance of AliveCheck was reloaded: instance_id: "
				<< instance_id << std::endl;
	}
}

AliveCheckConfig AliveCheckManager::parseConfiguration(long instance_id, std::map<std::string, std::string> configuration)
{
	AliveCheckConfig parsed_config;

	auto gateway_id_it = configuration.find("gateway_id");
	if (gateway_id_it == configuration.end()) {

		logger.LOGFILE("alive_check", "ERROR") << "Received configuration doesn't "
				"have member gateway_id." << std::endl;
		throw std::runtime_error("Could not parse received configuration.");
	}
	else {
		parsed_config.gateway_id = std::stoll(gateway_id_it->second);
		validateGatewayOwnership(instance_id, parsed_config.gateway_id);
	}

	auto send_notif_it = configuration.find("send_notif");
	if (send_notif_it == configuration.end()) {

		logger.LOGFILE("alive_check", "ERROR") << "Received configuration doesn't "
				"have member send_notif." << std::endl;
		throw std::runtime_error("Could not parse received configuration.");
	}
	else {
		parsed_config.send_notif = std::stoi(send_notif_it->second);
		validateSendNotif(parsed_config.send_notif);
	}
	return parsed_config;
}

void AliveCheckManager::validateGatewayOwnership(long instance_id, long long gateway_id)
{
	SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
	// Get ID of user owning instance.
	long user_id;
	*sql << "SELECT user_id FROM instance WHERE instance_id = :instance_id",
			soci::use(instance_id, "instance_id"),
			soci::into(user_id);

	short owns;
	*sql << "SELECT exists(SELECT 1 FROM user_gateway WHERE user_id = :user_id "
			"AND gateway_id = :gateway_id);",
			soci::use(user_id, "user_id"),
			soci::use(gateway_id, "gateway_id"),
			soci::into(owns);

	if (!owns) {
		logger.LOGFILE("alive_check", "ERROR") <<  "User with user_id: " << user_id <<
				" tried to operate with device which is on gateway user doesn't own: " << gateway_id << "." << std::endl;
		throw std::runtime_error("Could not process received configuration.");
	}
}

void AliveCheckManager::validateSendNotif(short send_notif)
{
	if (send_notif != 0 && send_notif != 1) {
		logger.LOGFILE("alive_check", "ERROR") << "send_notif of received configuration "
				"is not 0 or 1." << std::endl;
		throw std::runtime_error("Could not parse received configuration.");
	}
}
