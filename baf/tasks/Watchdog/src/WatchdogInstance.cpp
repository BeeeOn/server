/*
 * File:   WatchdogInstance.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 29. March 2016
 */

#include "WatchdogInstance.h"

#include <iostream>
#include <chrono>
#include <stdexcept>
#include <string>

#include <soci.h>

#include "../../../../Notificator/UriNotif.h"

#include "../../../src/DatabaseInterface.h"
#include "../../../src/GatewayInterface.h"
#include "../../../src/Logger.h"

#include "WatchdogManager.h"

WatchdogInstance::WatchdogInstance(long instance_id,
								   std::weak_ptr<TaskManager> owning_manager,
								   long device_euid):
	TaskInstance(instance_id, owning_manager),
	TriggerTaskInstance(instance_id, owning_manager),
	m_received_data_once(false)
{
	registerToReceiveDataFromDevice(device_euid);
}

WatchdogInstance::~WatchdogInstance()
{
}

void WatchdogInstance::run(DataMessage data_message)
{
	SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
	if (!m_received_data_once) {
		// Instance haven't received any data messege before, so it will just store it.
		soci::indicator ind_module_id;
		int module_id;

		*sql << "SELECT module_id FROM task_watchdog WHERE instance_id = :instance_id",
				soci::use(m_instance_id, "instance_id"),
				soci::into(module_id, ind_module_id);

		if (ind_module_id == soci::i_ok) {
			m_last_received_value = getModuleValue(module_id, data_message);
			m_received_data_once = true;
		}
		else {
			logger.LOGFILE("watchdog", "ERROR") << "Instance: " << m_instance_id
					<< ": module_id in configuration is not valid. " << std::endl;
		}
	}
	else {
		// Object to store configuration from database.
		soci::indicator ind_gateway_id, ind_device_euid, ind_module_id;
		long long gateway_id;
		long device_euid;
		int module_id;
		std::string comp_operator;
		double value;

		// Get base configuration of this instance from database.
		*sql << "SELECT gateway_id, device_euid, module_id, comp_operator, value FROM task_watchdog WHERE instance_id = :instance_id",
				soci::use(m_instance_id, "instance_id"),
				soci::into(gateway_id, ind_gateway_id),
				soci::into(device_euid, ind_device_euid),
				soci::into(module_id, ind_module_id),
				soci::into(comp_operator),
				soci::into(value);

		if (ind_device_euid == soci::i_ok && ind_module_id == soci::i_ok && ind_gateway_id == soci::i_ok) {

			double current_value = getModuleValue(module_id, data_message);

			if (comp_operator == "LT") {
				if ((current_value < m_last_received_value) &&
					(value <= m_last_received_value) &&
					(value >= current_value)) {

					if (shouldAct()) {
						// Condition of comp_operator was met.
						operatorConditionMet();
					}
				}
			}
			else if (comp_operator == "GT") {
				if ((current_value > m_last_received_value) &&
					(value <= current_value) &&
					(value >= m_last_received_value)) {

					if (shouldAct()) {
						// Condition of comp_operator was met.
						operatorConditionMet();
					}
				}
			}
			m_last_received_value = current_value;
		}
		else {
			logger.LOGFILE("watchdog", "ERROR") << "Instance: " << m_instance_id
					<< ": gateway_id, device_euid or module_id in configuration is not valid." << std::endl;
		}
	}
}

void WatchdogInstance::operatorConditionMet()
{
	SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();

	std::string type;
	*sql << "SELECT type FROM task_watchdog WHERE instance_id = :instance_id",
			soci::use(m_instance_id, "instance_id"),
			soci::into(type);

	if (type == "NOTIF" || type == "BOTH") {

		std::string notification;
		*sql << "SELECT notification FROM task_watchdog WHERE instance_id = :instance_id",
				soci::use(m_instance_id, "instance_id"),
				soci::into(notification);

		sendNotification(notification);
	}
	if (type == "SWITCH" || type == "BOTH") {

		switchActuator();
	}
}

void WatchdogInstance::sendNotification(std::string notification)
{
	long user_id;
	SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();

	// Get ids of all users who own this instance. This is prepared for future, when instance sharing will be possible.
	soci::rowset<soci::row> user_rows = (sql->prepare << "SELECT user_id FROM instance WHERE instance_id = :instance_id",
														  soci::use(m_instance_id, "instance_id"));

	for (soci::rowset<soci::row>::const_iterator user_it = user_rows.begin(); user_it != user_rows.end(); ++user_it) {

		std::vector<std::string> sr_ids;

		soci::row const& user_row = *user_it;
		// Get all user ids.
		user_id = user_row.get<int>(0);

		// Find all service_reference_ids binded with user.
		soci::rowset<soci::row> sri_rows = (sql->prepare << "SELECT service_reference_id "
				"FROM push_notification_service WHERE user_id = :user_id", soci::use(user_id, "user_id"));

		for (soci::rowset<soci::row>::const_iterator sri_it = sri_rows.begin(); sri_it != sri_rows.end(); ++sri_it) {

			soci::row const& sri_row = *sri_it;
			// Get all service_reference_ids.
			sr_ids.push_back(sri_row.get<std::string>(0));
		}
		long now_timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		// URI notif is just placeholder until AliveCheck notification is specified.
		std::shared_ptr<UriNotif> notif = std::make_shared<UriNotif>(user_id, m_instance_id, now_timestamp, notification, "");
		// Send notifications.
		notif->sendGcm(&sr_ids);
		logger.LOGFILE("watchdog", "INFO") << "Instance Watchdog: " << m_instance_id << " sent notifification: [user_id: " << user_id
				  << ", timestamp: " << now_timestamp << ", notification: " << notification << "]" << std::endl;
	}
}

void WatchdogInstance::switchActuator()
{
	SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();

	soci::indicator ind_a_gateway_id, ind_a_device_euid, ind_a_module_id;
	long long a_gateway_id;
	long a_device_euid;
	int a_module_id;
	int a_value;

	*sql << "SELECT a_gateway_id, a_device_euid, a_module_id, a_value "
			"FROM task_watchdog WHERE instance_id = :instance_id",
			soci::use(m_instance_id, "instance_id"),
			soci::into(a_gateway_id, ind_a_gateway_id),
			soci::into(a_device_euid, ind_a_device_euid),
			soci::into(a_module_id, ind_a_module_id),
			soci::into(a_value);

	// Changing of state of actuator can continue only if fetched values exist.
	if (ind_a_device_euid == soci::i_ok && ind_a_module_id == soci::i_ok && ind_a_gateway_id == soci::i_ok) {

		GatewayInterface gi;
		gi.sendSetState(a_gateway_id, a_device_euid, a_module_id, a_value);
		logger.LOGFILE("watchdog", "INFO") << "Instance Watchdog: " << m_instance_id << " switched actuator: [gateway_id: " << a_gateway_id << ", device_euid: "
				<< a_device_euid << ", module_id: " << a_module_id << ", value: " << a_value << "]" << std::endl;
		}
	else {
		logger.LOGFILE("watchdog", "ERROR") << "Instance: " << m_instance_id
				<< ": a_gateway_id, a_device_euid or a_module_id in configuration is not valid." << std::endl;
	}
}


double WatchdogInstance::getModuleValue(int module_id, DataMessage data_message)
{
	auto module_it = data_message.modules.find(module_id);
	if (module_it == data_message.modules.end()) {

		logger.LOGFILE("watchdog", "ERROR") << "Wanted module_id: " << module_id <<" was not found in"
				" received message: instance_id" << m_instance_id << std::endl;
		throw std::runtime_error("Run of watchdog was not successful.");
	}
	// Return received value.
	return module_it->second.second;
}

bool WatchdogInstance::shouldAct()
{
	// This construction is to protect from acting more than once in ten seconds
	// so it won't spam actuator or users device. In a case that value from module
	// oscilates around guarded value .
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	if (now < (m_last_act_time + std::chrono::seconds(10))) {
		 return false;
	}
	else {
		m_last_act_time = now;
		return true;
	}
}

void WatchdogInstance::changeRegisteredDeviceEuid(long device_euid)
{
	deleteFromControlComponent();
	registerToReceiveDataFromDevice(device_euid);
}
